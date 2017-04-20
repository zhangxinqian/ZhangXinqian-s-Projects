package modle;

public class MergeNode extends DiagramNode {

	public MergeNode(String mergeName, String xmiId, int localId) {
		super(mergeName, xmiId, localId);
		this.nodeType = NodeType.Merge;
	}
	
}
