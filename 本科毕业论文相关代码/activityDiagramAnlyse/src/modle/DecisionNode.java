package modle;

public class DecisionNode extends DiagramNode {

	private String documentation = null;//ע��Note����
	private String branchFuncCode = null;//������õ���uppaal�з�֧ѡ��ĺ����Ĵ���
	
	public DecisionNode(String decisionName, String xmiId, int localId, String documentation) {
		super(decisionName, xmiId, localId);
		this.documentation = documentation;
		this.nodeType = NodeType.Decision;
	}
	
	public String getDocumentation() {
		return documentation;
	}

	public void setDocumentation(String documentation) {
		this.documentation = documentation;
	}

	public String getBranchFuncCode() {
		return branchFuncCode;
	}

	public void setBranchFuncCode(String branchFuncCode) {
		this.branchFuncCode = branchFuncCode;
	}

}
