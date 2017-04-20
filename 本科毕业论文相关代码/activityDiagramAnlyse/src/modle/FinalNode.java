package modle;

public class FinalNode extends DiagramNode {

	public FinalNode(String finalName, String xmiId, int localId) {
		super(finalName, xmiId, localId);
		this.nodeType = NodeType.Final;
	}
	
}
