/**
 * @author ZhangXinqian
 * 使用Searcher对象进行微博内容的搜索；
 * 输入参数：建立好的索引文件目录的路径， 搜索的关键词；如：java -jar searcher.jar E:/weibo_index 高等数学
 * 关键词的检索结果(top20条微博)
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

	//设置以下几个私有变量时为了能初始化一个Searcher对象后，能够进行多次搜索
	
	//存储索引目录路径：
	private String indexDirectoryPath = null;
	//对微博内容进行解析的解析器：
	private QueryParser contentParser = null;
	//搜索器：
	private IndexSearcher searcher = null;

	/**
	 * 构造函数
	 * @param indexDirectoryPath 索引文件目录路径
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
	 * 用于通过关键词搜索微博内容的核心函数
	 * @param searchWords 搜索关键词
	 * @throws ParseException
	 * @throws IOException
	 */
	public void search(String searchWords) throws ParseException, IOException {

		//生成一条查询
		Query query = contentParser.parse(searchWords);

		//得到前（top）20条记录，使用Lucene内部自己的rank评分机制得到排名最靠前的20条微博数据
		TopDocs topdocs = searcher.search(query, 20);
		ScoreDoc[] scoreDocs = topdocs.scoreDocs;
		System.out.println("一共查到：" + topdocs.totalHits + "记录，只 显示前20条微博内容：");

		//显示排名最靠前的20条微博内容：
		for (int i = 0; i < scoreDocs.length; i++) {
			int doc = scoreDocs[i].doc;
			Document document = searcher.doc(doc);
			System.out.println("微博消息：" + document.get("username") + "\t"
					+ document.get("date") + "\t" + document.get("content"));
		}

	}

	/**
	 * 初始化搜索器
	 * @throws IOException
	 */
	private void initialize() throws IOException {
		//打开并载入硬盘里的索引文件，建立搜索器
		Directory indexDir = FSDirectory
				.open(new File(this.indexDirectoryPath));
		IndexReader reader = DirectoryReader.open(indexDir);
		searcher = new IndexSearcher(reader);
		// reader.close();注：这里reader不能被关闭。

		//创建内容解析器，使用lucene版本为 4.6，由于微博内容是中文，故使用SmartChineseAnalyzer作为分词器
		contentParser = new QueryParser(Version.LUCENE_46, "content",
				new SmartChineseAnalyzer(Version.LUCENE_46, true));
	}

	/**
	 * 入口main函数
	 * @param args 索引文件目录的路径，以及 搜索关键词
	 */
	public static void main(String[] args) {
		//Searcher searcher = new Searcher("E:\\MyEclipseWorkspace\\weibo_index");// 注：之前测试用的
		Searcher searcher = new Searcher(args[0]);
		try {
			//searcher.search("华东师范大学");//注：之前测试用的
			searcher.search(args[1]);
		} catch (ParseException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
}
