package modle;

public class InitNode extends DiagramNode {
	
	private String documentation = null;//注释Note文字
	private String variableDeclarationCode = null;//全局变量声明
	private String initFunctionCode = null;//初始化函数代码
	
	public InitNode(String initName, String xmiId, int localId) {
		super(initName, xmiId, localId);
		this.nodeType = NodeType.Init;
	}
	
	public InitNode(String initName, String xmiId, int localId, String documentation) {
		super(initName, xmiId, localId);
		this.nodeType = NodeType.Init;
		this.documentation = documentation;
	}
	
	public String getDocumentation() {
		return documentation;
	}

	public void setDocumentation(String documentation) {
		this.documentation = documentation;
	}

	public String getVariableDeclarationCode() {
		return variableDeclarationCode;
	}

	public void setVariableDeclarationCode(String variableDeclarationCode) {
		this.variableDeclarationCode = variableDeclarationCode;
	}

	public String getInitFunctionCode() {
		return initFunctionCode;
	}

	public void setInitFunctionCode(String initFunctionCode) {
		this.initFunctionCode = initFunctionCode;
	}
}
