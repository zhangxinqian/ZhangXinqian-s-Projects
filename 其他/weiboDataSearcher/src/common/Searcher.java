/**
 * @author ZhangXinqian
 * ʹ��Searcher�������΢�����ݵ�������
 * ��������������õ������ļ�Ŀ¼��·���� �����Ĺؼ��ʣ��磺java -jar searcher.jar E:/weibo_index �ߵ���ѧ
 * �ؼ��ʵļ������(top20��΢��)
 */

package common;

import java.io.File;
import java.io.IOException;

import org.apache.lucene.analysis.cn.smart.SmartChineseAnalyzer;
import org.apache.lucene.document.Document;
import org.apache.lucene.index.DirectoryReader;
import org.apache.lucene.index.IndexReader;
import org.apache.lucene.queryparser.classic.ParseException;
import org.apache.lucene.queryparser.classic.QueryParser;
import org.apache.lucene.search.IndexSearcher;
import org.apache.lucene.search.Query;
import org.apache.lucene.search.ScoreDoc;
import org.apache.lucene.search.TopDocs;
import org.apache.lucene.store.Directory;
import org.apache.lucene.store.FSDirectory;
import org.apache.lucene.util.Version;

public class Searcher {

	//�������¼���˽�б���ʱΪ���ܳ�ʼ��һ��Searcher������ܹ����ж������
	
	//�洢����Ŀ¼·����
	private String indexDirectoryPath = null;
	//��΢�����ݽ��н����Ľ�������
	private QueryParser contentParser = null;
	//��������
	private IndexSearcher searcher = null;

	/**
	 * ���캯��
	 * @param indexDirectoryPath �����ļ�Ŀ¼·��
	 */
	public Searcher(String indexDirectoryPath) {
		this.indexDirectoryPath = indexDirectoryPath;
		try {
			initialize();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	/**
	 * ����ͨ���ؼ�������΢�����ݵĺ��ĺ���
	 * @param searchWords �����ؼ���
	 * @throws ParseException
	 * @throws IOException
	 */
	public void search(String searchWords) throws ParseException, IOException {

		//����һ����ѯ
		Query query = contentParser.parse(searchWords);

		//�õ�ǰ��top��20����¼��ʹ��Lucene�ڲ��Լ���rank���ֻ��Ƶõ������ǰ��20��΢������
		TopDocs topdocs = searcher.search(query, 20);
		ScoreDoc[] scoreDocs = topdocs.scoreDocs;
		System.out.println("һ���鵽��" + topdocs.totalHits + "��¼��ֻ ��ʾǰ20��΢�����ݣ�");

		//��ʾ�����ǰ��20��΢�����ݣ�
		for (int i = 0; i < scoreDocs.length; i++) {
			int doc = scoreDocs[i].doc;
			Document document = searcher.doc(doc);
			System.out.println("΢����Ϣ��" + document.get("username") + "\t"
					+ document.get("date") + "\t" + document.get("content"));
		}

	}

	/**
	 * ��ʼ��������
	 * @throws IOException
	 */
	private void initialize() throws IOException {
		//�򿪲�����Ӳ����������ļ�������������
		Directory indexDir = FSDirectory
				.open(new File(this.indexDirectoryPath));
		IndexReader reader = DirectoryReader.open(indexDir);
		searcher = new IndexSearcher(reader);
		// reader.close();ע������reader���ܱ��رա�

		//�������ݽ�������ʹ��lucene�汾Ϊ 4.6������΢�����������ģ���ʹ��SmartChineseAnalyzer��Ϊ�ִ���
		contentParser = new QueryParser(Version.LUCENE_46, "content",
				new SmartChineseAnalyzer(Version.LUCENE_46, true));
	}

	/**
	 * ���main����
	 * @param args �����ļ�Ŀ¼��·�����Լ� �����ؼ���
	 */
	public static void main(String[] args) {
		//Searcher searcher = new Searcher("E:\\MyEclipseWorkspace\\weibo_index");// ע��֮ǰ�����õ�
		Searcher searcher = new Searcher(args[0]);
		try {
			//searcher.search("����ʦ����ѧ");//ע��֮ǰ�����õ�
			searcher.search(args[1]);
		} catch (ParseException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
}
