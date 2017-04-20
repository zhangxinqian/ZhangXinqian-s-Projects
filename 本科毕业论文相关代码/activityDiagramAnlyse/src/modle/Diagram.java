package modle;

import java.util.LinkedList;

import modle.ActivityNode.DistributionType;
import modle.DiagramNode.NodeType;

public class Diagram {
	
	private LinkedList<DiagramNode> nodeList = new LinkedList<DiagramNode>();
	
	public Diagram() {
		
	}
	
	public void addNode(DiagramNode node) {
		nodeList.add(node);
	}
	
	public void addTransition(int sourceLocalId, int targetLocalId) {
		DiagramNode sourceNode = getNodeByLocalId(sourceLocalId);
		DiagramNode targetNode = getNodeByLocalId(targetLocalId);
		if (sourceNode == null) 
		{
			System.out.println("Source Node Not Exist " + sourceLocalId);
		}
		if (targetNode == null) 
		{
			System.out.println("Target Node Not Exist " + targetLocalId);
		}
		sourceNode.addSendNode(targetNode);
		targetNode.addReceiveNode(sourceNode);
	}
	
	public DiagramNode getNodeByLocalId(int localId) {
		for (DiagramNode i : nodeList)
		{
			if (i.getLocalId() == localId)
			{
				return i;
			}
		}
		return null;
	}
	
	public DiagramNode getNodeByNumber(int number) {
		for (DiagramNode i : nodeList)
		{
			if (i.getNumber() == number)
			{
				return i;
			}
		}
		return null;
	}
	
	public DiagramNode getNodeByNodeName(String nodeName) {
		for (DiagramNode i : nodeList)
		{
			if (i.getNodeName().equals(nodeName))
			{
				return i;
			}
		}
		return null;
	}
	
	public int getNodeNumberByNodeName(String nodeName) {
		
		DiagramNode node = getNodeByNodeName(nodeName);
		if (node != null)
		{
			return node.getNumber();
		}
		return -1;
	}
	
	public int getNodeCount() {
		return nodeList.size();
	}
	
	public int getMaxOut() {
		
		int maxOut = 0;
		int tempInt = maxOut; 
		
		for (DiagramNode i : nodeList)
		{
			tempInt = i.getSendCount();
			if (maxOut < tempInt)
			{
				maxOut = tempInt;
			}
			tempInt = i.getReceiveCount();
			if (maxOut < tempInt)
			{
				maxOut = tempInt;
			}
		}
		
		return maxOut;
		
	}
	
	//所有节点加入完毕，才能对一个完整的活动图进行解析
	public void parseDiagram() {
		parseInitNode();
		parseActivityNodes();
		parseDecisionNodes();
	}
	
	private void parseInitNode() {
		
		String documentation = null;
		String variableDeclarationCode = "";
		String initFunctionCode = "";
		InitNode initNode = null;
		for (DiagramNode i : nodeList)
		{
			if (i.getNodeType() == NodeType.Init)
			{
				initNode = (InitNode)i;
				break;
			}
		}
		if (initNode != null)
		{
			documentation = initNode.getDocumentation();
			if (documentation != null && !documentation.equals(""))
			{
				int dI = documentation.indexOf("Declaration:");
				int fI = documentation.indexOf("Function:");
				if (dI != -1 && fI != -1) 
				{
					variableDeclarationCode = documentation.substring(dI+12, fI) + "\n\n";
					initFunctionCode = documentation.substring(fI+9);
					initFunctionCode = "void init_func()\n{\n" + initFunctionCode + "\n}\n\n";
				}
				else if (dI != -1 && fI == -1)
				{
					variableDeclarationCode = documentation.substring(dI+12) + "\n\n";
					initFunctionCode = "";
				}
				else if (dI == -1 && fI != -1)
				{
					variableDeclarationCode = "";
					initFunctionCode = documentation.substring(fI+9);
					initFunctionCode = "void init_func()\n{\n" + initFunctionCode + "\n}\n\n";
				}
			}
			initNode.setVariableDeclarationCode(normalizeString(variableDeclarationCode));
			initNode.setInitFunctionCode(normalizeString(initFunctionCode));
		}		
		
	}
	
	private void parseActivityNodes() {
		
		for (DiagramNode i : nodeList)
		{
			if (i.getNodeType() == NodeType.Activity)
			{
				ActivityNode activityNode = (ActivityNode)i;
				String documentation = activityNode.getDocumentation();
				String meanOfRunningTime = "";
				String deviationOfRunningTime = "";
				String activityFuncCode = "";
				DistributionType distributionType = null;
				
				if (documentation != null && !documentation.equals(""))
				{
					int iDistri = documentation.indexOf("Distribution Type:");
					int iMean = documentation.indexOf("Mean:");
					int iDevi = documentation.indexOf("Deviation:");
					int iFunc = documentation.indexOf("Function:");
					if (iDistri != -1)
					{
						String typeString = documentation.substring(iDistri+18, documentation.indexOf(";", iDistri+18));
						if (typeString.equals("Normal"))
						{
							distributionType = DistributionType.Normal;
						}
						//else 其他分布以后考虑
					}
					if (iMean != -1)
					{
						meanOfRunningTime = documentation.substring(iMean+5, documentation.indexOf(";", iMean+5));
					}
					if (iDevi != -1)
					{
						deviationOfRunningTime = documentation.substring(iDevi+10, documentation.indexOf(";", iDevi+10));
					}
					if (iFunc != -1)
					{
						activityFuncCode = documentation.substring(iFunc+9);
						activityFuncCode = "//Activity " + activityNode.getNodeName() + 
								" function:\nvoid activity_"+ activityNode.getNumber() +
								"_func()\n{\n" + activityFuncCode + "\n}\n\n";
						activityFuncCode = normalizeString(activityFuncCode);
					}
				}
				activityNode.setDistributionType(distributionType);
				activityNode.setMeanOfRunningTime(meanOfRunningTime);
				activityNode.setDeviationOfRunningTime(deviationOfRunningTime);
				activityNode.setActivitFuncCode(activityFuncCode);
			}
		}
		
	}
	
	private void parseDecisionNodes() {
		
		OK:
		for (DiagramNode i : nodeList)
		{
			if (i.getNodeType() == NodeType.Decision)
			{
				DecisionNode decisionNode = (DecisionNode)i;
				String documentation = decisionNode.getDocumentation();
				String branchFuncCode = "";
				
				if (documentation != null && !documentation.equals(""))
				{
					int fI = documentation.indexOf("Function:");
					if (fI == -1)
					{
						decisionNode.setBranchFuncCode("");
						continue OK;
					}
					documentation = documentation.substring(fI+9);
					
					String strArr[] = documentation.split("return ");
					String nodeName;
					int nodeNum;
					String branchName;
					if (strArr.length > 1) 
					{
						for (int j = 1; j < strArr.length; j++)
						{
							nodeName = strArr[j].substring(0, strArr[j].indexOf(";"));
							nodeNum = getNodeNumberByNodeName(nodeName);
							if (nodeNum == -1)
							{
								decisionNode.setBranchFuncCode("");
								continue OK;
							}
							branchName = "encode_msg(" + decisionNode.getNumber() + ", " + nodeNum + ")";
							strArr[j] = strArr[j].replaceFirst(nodeName, branchName);
						}
						for (int j = 0; j < strArr.length-1; j++)
						{
							branchFuncCode = branchFuncCode + strArr[j] + "return ";
						}
						branchFuncCode += strArr[strArr.length-1];
						
						branchFuncCode = "//Branch " + decisionNode.getNodeName() + " function:\nmessage_t branch_" 
								+ decisionNode.getNumber() + "_func(id_t nid)\n{\n" + branchFuncCode + "\n}\n\n";
						branchFuncCode = normalizeString(branchFuncCode);
					}
					else
					{
						decisionNode.setBranchFuncCode("");
						continue OK;
					}
					
				}
				decisionNode.setBranchFuncCode(branchFuncCode);
			}
		}
		
	}
	
	public void printDiagram() {
		for (DiagramNode i : nodeList) 
		{
			i.printDiagramNode();
		}
	}
	
	private String normalizeString(String str) {
		
		String resultString = str.replace("&gt;", ">");
		resultString = resultString.replace("&lt;", "<");
		resultString = resultString.replace("&amp;", "&");
		resultString = resultString.replace("&quot;", "\"");
		resultString = resultString.replace("&apos;", "'");
		
		return resultString;
	}
}
