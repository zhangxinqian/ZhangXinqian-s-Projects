package modle;

public class DecisionNode extends DiagramNode {

	private String documentation = null;//注释Note文字
	private String branchFuncCode = null;//解析后得到的uppaal中分支选择的函数的代码
	
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
