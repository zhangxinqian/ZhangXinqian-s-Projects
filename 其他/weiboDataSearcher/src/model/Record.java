package model;

/**
 * @author ZhangXinqian
 * ʹ��Record����洢һ��΢�����ݣ�����Id���û�������������ʱ�������
 */

public class Record {
	private String id=null;
	private String userName=null;
	private String date=null;
	private String content=null;
	
	public Record(){}
	public Record(String id,String userName,String date,String content){
		this.setId(id);
		this.setUserName(userName);
		this.setDate(date);
		this.setContent(content);
		
	}
	public String getId() {
		return id;
	}
	public void setId(String id) {
		this.id = id;
	}
	public String getUserName() {
		return userName;
	}
	public void setUserName(String userName) {
		this.userName = userName;
	}
	public String getDate() {
		return date;
	}
	public void setDate(String date) {
		this.date = date;
	}
	public String getContent() {
		return content;
	}
	public void setContent(String content) {
		this.content = content;
	}

}
