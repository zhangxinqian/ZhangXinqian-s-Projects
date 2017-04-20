package modle;

public class ActivityNode extends DiagramNode {

	enum DistributionType {Normal, Uniform, Exponential, Poisson}
	
	private String documentation = null;//ע��Note����
	private String activityFuncCode = null;//������õ���uppaal�лִ�еĺ����Ĵ���
	private DistributionType distributionType = null; //Running time ��ʱ����ʷֲ�����
	private String meanOfRunningTime = null;
	private String deviationOfRunningTime = null;
	
	public ActivityNode(String activityName, String xmiId, int locaId, String documentation) {
		super(activityName, xmiId, locaId);
		this.setDocumentation(documentation);
		this.nodeType = NodeType.Activity;
	}

	public String getDocumentation() {
		return documentation;
	}

	public void setDocumentation(String documentation) {
		this.documentation = documentation;
	}

	public String getMeanOfRunningTime() {
		return meanOfRunningTime;
	}

	public void setMeanOfRunningTime(String meanOfRunningTime) {
		this.meanOfRunningTime = meanOfRunningTime;
	}

	public String getDeviationOfRunningTime() {
		return deviationOfRunningTime;
	}

	public void setDeviationOfRunningTime(String deviationOfRunningTime) {
		this.deviationOfRunningTime = deviationOfRunningTime;
	}

	public DistributionType getDistributionType() {
		return distributionType;
	}

	public void setDistributionType(DistributionType distributionType) {
		this.distributionType = distributionType;
	}

	public String getActivitFuncCode() {
		return activityFuncCode;
	}

	public void setActivitFuncCode(String activityFuncCode) {
		this.activityFuncCode = activityFuncCode;
	}
	
}
