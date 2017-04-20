package modle;

import java.util.LinkedList;
import java.util.List;

public class DiagramNode {
	
	public enum NodeType {Activity, Init, Final, Decision, Merge, Synchronization}
	
	protected String nodeName;//节点名称
	protected NodeType nodeType = null;
	protected int number;//活动图节点编号
	
	protected String xmiId;//在xmi中节点的唯一编号
	protected int localId;//在EA中的位置标号
	
	protected List<DiagramNode> receiveNodes = new LinkedList<DiagramNode>();
	protected List<DiagramNode> sendNodes = new LinkedList<DiagramNode>();
	
	public DiagramNode() {
		// TODO Auto-generated constructor stub
		
	}
	
	public DiagramNode(String nodeName, String xmiId, int localId) {
		this.setNodeName(nodeName);
		this.setXmiId(xmiId);
		this.setLocalId(localId);
	}

	public String getNodeName() {
		return nodeName;
	}
	
	public void setNodeName(String nodeName) {
		this.nodeName = nodeName;
	}
	
	public NodeType getNodeType() {
		return nodeType;
	}
	
	public int getNumber() {
		return number;
	}
	
	public void setNumber(int number) {
		this.number = number;
	}
	
	public String getXmiId() {
		return xmiId;
	}

	public void setXmiId(String xmiId) {
		this.xmiId = xmiId;
	}

	public int getLocalId() {
		return localId;
	}

	public void setLocalId(int localId) {
		this.localId = localId;
	}
	
	public void addReceiveNode(DiagramNode node) {
		receiveNodes.add(node);
	}
	
	public void addSendNode(DiagramNode node) {
		sendNodes.add(node);
	}
	
	public int getReceiveCount() {
		return receiveNodes.size();
	}
	
	//index为list中的下标，取出第index个sendNode
	public DiagramNode getSendNode(int index) {
		DiagramNode node = sendNodes.get(index);
		return node;
	}
	
	public DiagramNode getReceiveNode(int index) {
		DiagramNode node = receiveNodes.get(index);
		return node;
	}
	
	public int getSendNodeNumber(int index) {
		return getSendNode(index).getNumber();
	}
	
	public int getReveiveNodeNumber(int index) {
		return getReceiveNode(index).getNumber();
	}
	
	public int getSendCount() {
		return sendNodes.size();
	}
	
	public void printDiagramNode() {
		System.out.println("number:"+number+" localId:"+localId+" receiveCount:"+getReceiveCount()+" sendCount:"+getSendCount());
	}
}
