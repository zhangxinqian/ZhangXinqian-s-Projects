/***
 * @author ZhangXinqian
 * 使用Indexer类建立微博数据索引；
 * 只需要对微博源数据建立一次索引即可，时间10到15分钟（1300000多条微博）
 * 输入参数为 索引目录的目标路径  以及 微博数据目录路径 ；如：java -jar indexer.jar E:/wbIndx E:/微博数据.txt
 * 输出： 在索引目录目标路径下生成索引文件
 */

package common;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStreamReader;

import org.apache.lucene.analysis.Analyzer;
import org.apache.lucene.analysis.cn.smart.SmartChineseAnalyzer;
import org.apache.lucene.document.Document;
import org.apache.lucene.document.Field.Store;
import org.apache.lucene.document.StringField;
import org.apache.lucene.document.TextField;
import org.apache.lucene.index.IndexWriter;
import org.apache.lucene.index.IndexWriterConfig;
import org.apache.lucene.store.Directory;
import org.apache.lucene.store.FSDirectory;
import org.apache.lucene.util.Version;

import model.Record;

public class Indexer {

	/**
	 * 
	 * @param indexDirectoryPath
	 *            要生成的索引的文件目录路径
	 * @param dataFilePath
	 *            微博数据的目录路径
	 * @return 索引个数
	 * @throws IOException
	 */
	public int index(String indexDirectoryPath, String dataFilePath)
			throws IOException {
		File dataFile = new File(dataFilePath);
		File indexDir = new File(indexDirectoryPath);

		if (!dataFile.exists()) {
			throw new IOException("微博数据文件不存在");
		}

		/*
		 * 在硬盘里建立文件索引
		 */

		// 使用Lucene4.6、 SmartChineseAnalyzer中文分词器
		Analyzer analyzer = new SmartChineseAnalyzer(Version.LUCENE_46, true);

		IndexWriterConfig indexWriterConfig = new IndexWriterConfig(
				Version.LUCENE_46, analyzer);

		Directory directory = FSDirectory.open(indexDir);
		if (IndexWriter.isLocked(directory)) {
			IndexWriter.unlock(directory);
		}

		// 通过配置设置好的配置信息和文件目录路径，创建 索引写入器
		IndexWriter writer = new IndexWriter(directory, indexWriterConfig);
		writer.deleteAll();// 清空目录下原来的索引

		// 调用子函数来实现 建立索引
		indexRecords(writer, dataFile);

		int numIndexed = writer.numDocs();
		writer.close();

		return numIndexed;
	}

	/**
	 * 建立索引子函数
	 * 
	 * @param writer
	 *            索引写入器
	 * @param dataFile
	 *            微博数据目录路径
	 * @throws IOException
	 */
	private void indexRecords(IndexWriter writer, File dataFile)
			throws IOException {
		// 初始化工作：
		FileInputStream fileInputStream = new FileInputStream(dataFile);
		InputStreamReader inputStreamReader = new InputStreamReader(
				fileInputStream, "utf-8");
		BufferedReader reader = new BufferedReader(inputStreamReader);

		Record record = null;

		// 循环：对一条条读取的微博数据 建立索引
		while ((record = readOneRecord(reader)) != null) {
			System.out.println("微博" + record.getId() + "正在被索引。");
			Document doc = new Document();

			/*
			 * 对一条微博数据建立一个文档，每个文档分为 id，username，date和content四大域；
			 * 为了后面搜索获取内容方便，加上微博内容较短（140字以内），将微博内容也存储在索引文件中；
			 * （在实际应用中，正常是通过存储的路径来找到文件的）
			 */
			doc.add(new StringField("id", record.getId(), Store.YES));
			doc.add(new StringField("username", record.getUserName(), Store.YES));
			doc.add(new StringField("date", record.getDate(), Store.YES));
			// 内容较多，使用TextField
			doc.add(new TextField("content", record.getContent(), Store.YES));

			writer.addDocument(doc);// 索引写入器 写入一条索引
		}

		reader.close();
	}

	/**
	 * 获取一条微博数据
	 * 
	 * @param reader
	 * @return 返回一条记录，使用Record对象作为数据结构
	 * @throws IOException
	 */
	private Record readOneRecord(BufferedReader reader) throws IOException {
		String line = null;
		Record record = null;
		while ((line = reader.readLine()) != null) {
			// 原微博中是使用'\t'来分割每条数据的，所以可以这样获取每条数据中的相关内容
			String[] strArr = line.split("\t");
			//只处理格式正确的微博：ID，username，date和content 4个内容都有的值；
			if (strArr.length == 4) {
				record = new Record(strArr[0], strArr[1], strArr[2], strArr[3]);
				break;
			} else {
				continue;
			}
		}
		return record;
	}

	/**
	 * 入口main函数
	 * 
	 * @param args
	 *            微博数据的目录路径 && 要生成的索引的文件目录路径
	 */
	public static void main(String[] args) {
		Indexer indexer = new Indexer();
		try {
			// int num=indexer.index("E:\\weiboDataSearcher\\weibo_index",
			// "E:\\weiboDataSearcher\\weibo_data.txt"); 注：用于测试
			int num = indexer.index(args[0], args[1]);
			System.out.println("共索引了" + num + "篇微博");
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
}
