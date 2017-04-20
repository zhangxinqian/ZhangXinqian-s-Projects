package modle;

public class SynchronizationNode extends DiagramNode {

	public SynchronizationNode(String syncName, String xmiId, int localId) {
		super(syncName, xmiId, localId);
		this.nodeType = NodeType.Synchronization;
	}
	
}
