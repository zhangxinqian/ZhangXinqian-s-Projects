package parser;

import java.io.File;
import java.util.Iterator;
import java.util.List;

import modle.ActivityNode;
import modle.DecisionNode;
import modle.Diagram;
import modle.DiagramNode;
import modle.FinalNode;
import modle.InitNode;
import modle.MergeNode;
import modle.SynchronizationNode;

import org.dom4j.Document;
import org.dom4j.DocumentException;
import org.dom4j.Element;
import org.dom4j.Node;
import org.dom4j.io.SAXReader;

public class XMIParser {

	private static String activityFilterString = "/XMI/XMI.content/UML:Model/UML:Namespace.ownedElement/" 
			+ "UML:Package/UML:Namespace.ownedElement/UML:ActivityModel/UML:StateMachine.top/"
			+ "UML:CompositeState/UML:CompositeState.substate/UML:ActionState";
	
	private static String pseudoFilterString = "/XMI/XMI.content/UML:Model/UML:Namespace.ownedElement/"
			+ "UML:Package/UML:Namespace.ownedElement/UML:ActivityModel/UML:StateMachine.top/"
			+ "UML:CompositeState/UML:CompositeState.substate/UML:PseudoState";
	
	private static String transitionFilterString = "/XMI/XMI.content/UML:Model/UML:Namespace.ownedElement/"
			+ "UML:Package/UML:Namespace.ownedElement/UML:ActivityModel/UML:StateMachine.transitions/"
			+ "UML:Transition/UML:ModelElement.taggedValue";
	
	
	public XMIParser() {
		
	}
	
	public Diagram parse(String filePath) throws DocumentException {
	
		Diagram diagram = new Diagram();
		
		SAXReader saxReader = new SAXReader();
		Document document = saxReader.read(new File(filePath));
		
		parseActivity(document, diagram);
		parsePseudoNodes(document, diagram);
		parseTransitions(document, diagram);
		
		diagram.parseDiagram();
		
		return diagram;
	}
	
	private void parseActivity(Document document, Diagram diagram) {
		@SuppressWarnings("unchecked")
		List<Node> activitiesList = document.selectNodes(activityFilterString);
		int number = diagram.getNodeCount();		
		
		for (Iterator<Node> i = activitiesList.iterator(); i.hasNext(); )
		{
			Element element = (Element) i.next();
			String name = element.attributeValue("name");
			String xmiId= element.attributeValue("xmi.id");
			String type = null;
			String documentation = null;
			int localId = 0;
			
			@SuppressWarnings("unchecked")
			Iterator<Node> j = element.elementIterator();
			if (j.hasNext()) 
			{
				Element element2 = (Element) j.next();
				for (@SuppressWarnings("unchecked")
				Iterator<Node> k = element2.elementIterator(); k.hasNext(); ) 
				{
					Element element3 = (Element) k.next();
					String value = element3.attributeValue("tag");
					if (value.equals("ea_stype")) 
					{
						type = element3.attributeValue("value");
					} 
					else if (value.equals("ea_localid")) 
					{
						localId = Integer.parseInt(element3.attributeValue("value"));
					}
					else if (value.equals("documentation"))
					{
						documentation = element3.attributeValue("value");
					}
				}
				
				if (type.equals("Activity") || type.equals("Action")) 
				{
					DiagramNode node = new ActivityNode(name, xmiId, localId, documentation);
					node.setNumber(number);
					diagram.addNode(node);
					number ++;
					//System.out.println("Activities Name: "+((ActivityNode) node).getActivityName());
				}
				else 
				{
					System.out.println("Unrecongized Node: " + type + "\t" + name + "\t");
				}
			}
		}
	}
	
	private void parsePseudoNodes(Document document, Diagram diagram) {
		@SuppressWarnings("unchecked")
		List<Node> pseudoNodeList = document.selectNodes(pseudoFilterString);
		int number = diagram.getNodeCount();
		
		for (Iterator<Node> i = pseudoNodeList.iterator(); i.hasNext(); )
		{
			Element element = (Element) i.next();
			String name = element.attributeValue("name");
			String kind = element.attributeValue("kind");
			String xmiId= element.attributeValue("xmi.id");
			String type = null;
			String documentation = null;
			int localId = 0;
			
			@SuppressWarnings("unchecked")
			Iterator<Node> j = element.elementIterator();
			if (j.hasNext()) 
			{
				Element element2 = (Element) j.next();
				for (@SuppressWarnings("unchecked")
				Iterator<Node> k = element2.elementIterator(); k.hasNext(); ) 
				{
					Element element3 = (Element) k.next();
					String value = element3.attributeValue("tag");
					if (value.equals("ea_stype")) 
					{
						type = element3.attributeValue("value");
					} 
					else if (value.equals("ea_localid")) 
					{
						localId = Integer.parseInt(element3.attributeValue("value"));
					}
					else if (value.equals("documentation"))
					{
						documentation = element3.attributeValue("value");
					}
				}
				
				if (type.equals("Decision")) 
				{
					DiagramNode node = new DecisionNode(name, xmiId, localId, documentation);
					node.setNumber(number);
					diagram.addNode(node);
					number ++;
				}
				else if (type.equals("Synchronization"))
				{
					DiagramNode node = new SynchronizationNode(name, xmiId, localId);
					node.setNumber(number);
					diagram.addNode(node);
					number ++;
				}
				else if (type.equals("StateNode"))
				{
					if (kind != null && kind.equals("final"))
					{
						DiagramNode node = new FinalNode(name, xmiId, localId);
						node.setNumber(number);
						diagram.addNode(node);
						number ++;
					}
					else
					{
						DiagramNode node = new InitNode(name, xmiId, localId, documentation);
						node.setNumber(number);
						diagram.addNode(node);
						number ++;
					}
				}
				else if (type.equals("MergeNode"))
				{
					DiagramNode node = new MergeNode(name, xmiId, localId);
					node.setNumber(number);
					diagram.addNode(node);
					number ++;
				}
				else 
				{
					System.out.println("Unrecongized Node: " + type + "\t");
				}
			}
		}
	}
	
	private void parseTransitions(Document document, Diagram diagram) {
		
		@SuppressWarnings("unchecked")
		List<Node> transitionsList = document.selectNodes(transitionFilterString);
		
		for (Iterator<Node> i = transitionsList.iterator(); i.hasNext(); ) 
		{
			Element e = (Element) i.next();
			String sourceID = null;
			String targetID = null;
			for (@SuppressWarnings("unchecked")
			Iterator<Node> j = e.elementIterator(); j.hasNext(); ) {
				Element e1 = (Element) j.next();
				String value = e1.attributeValue("tag");
				if (value.equals("ea_sourceID")) {
					sourceID = e1.attributeValue("value");
				}
				if (value.equals("ea_targetID")) {
					targetID = e1.attributeValue("value");
				}
			}
			diagram.addTransition(Integer.parseInt(sourceID), Integer.parseInt(targetID));
		}
	}

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		XMIParser parser = new XMIParser();
		//Diagram diagram = parser.parse("./data/sample.xml");
		Diagram diagram = null;
		try {
			diagram = parser.parse("./data/case_stock.xml");
			diagram.printDiagram();
		} catch (DocumentException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

}
