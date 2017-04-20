/***
 * @author ZhangXinqian
 * ʹ��Indexer�ཨ��΢������������
 * ֻ��Ҫ��΢��Դ���ݽ���һ���������ɣ�ʱ��10��15���ӣ�1300000����΢����
 * �������Ϊ ����Ŀ¼��Ŀ��·��  �Լ� ΢������Ŀ¼·�� ���磺java -jar indexer.jar E:/wbIndx E:/΢������.txt
 * ����� ������Ŀ¼Ŀ��·�������������ļ�
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
	 *            Ҫ���ɵ��������ļ�Ŀ¼·��
	 * @param dataFilePath
	 *            ΢�����ݵ�Ŀ¼·��
	 * @return ��������
	 * @throws IOException
	 */
	public int index(String indexDirectoryPath, String dataFilePath)
			throws IOException {
		File dataFile = new File(dataFilePath);
		File indexDir = new File(indexDirectoryPath);

		if (!dataFile.exists()) {
			throw new IOException("΢�������ļ�������");
		}

		/*
		 * ��Ӳ���ｨ���ļ�����
		 */

		// ʹ��Lucene4.6�� SmartChineseAnalyzer���ķִ���
		Analyzer analyzer = new SmartChineseAnalyzer(Version.LUCENE_46, true);

		IndexWriterConfig indexWriterConfig = new IndexWriterConfig(
				Version.LUCENE_46, analyzer);

		Directory directory = FSDirectory.open(indexDir);
		if (IndexWriter.isLocked(directory)) {
			IndexWriter.unlock(directory);
		}

		// ͨ���������úõ�������Ϣ���ļ�Ŀ¼·�������� ����д����
		IndexWriter writer = new IndexWriter(directory, indexWriterConfig);
		writer.deleteAll();// ���Ŀ¼��ԭ��������

		// �����Ӻ�����ʵ�� ��������
		indexRecords(writer, dataFile);

		int numIndexed = writer.numDocs();
		writer.close();

		return numIndexed;
	}

	/**
	 * ���������Ӻ���
	 * 
	 * @param writer
	 *            ����д����
	 * @param dataFile
	 *            ΢������Ŀ¼·��
	 * @throws IOException
	 */
	private void indexRecords(IndexWriter writer, File dataFile)
			throws IOException {
		// ��ʼ��������
		FileInputStream fileInputStream = new FileInputStream(dataFile);
		InputStreamReader inputStreamReader = new InputStreamReader(
				fileInputStream, "utf-8");
		BufferedReader reader = new BufferedReader(inputStreamReader);

		Record record = null;

		// ѭ������һ������ȡ��΢������ ��������
		while ((record = readOneRecord(reader)) != null) {
			System.out.println("΢��" + record.getId() + "���ڱ�������");
			Document doc = new Document();

			/*
			 * ��һ��΢�����ݽ���һ���ĵ���ÿ���ĵ���Ϊ id��username��date��content�Ĵ���
			 * Ϊ�˺���������ȡ���ݷ��㣬����΢�����ݽ϶̣�140�����ڣ�����΢������Ҳ�洢�������ļ��У�
			 * ����ʵ��Ӧ���У�������ͨ���洢��·�����ҵ��ļ��ģ�
			 */
			doc.add(new StringField("id", record.getId(), Store.YES));
			doc.add(new StringField("username", record.getUserName(), Store.YES));
			doc.add(new StringField("date", record.getDate(), Store.YES));
			// ���ݽ϶࣬ʹ��TextField
			doc.add(new TextField("content", record.getContent(), Store.YES));

			writer.addDocument(doc);// ����д���� д��һ������
		}

		reader.close();
	}

	/**
	 * ��ȡһ��΢������
	 * 
	 * @param reader
	 * @return ����һ����¼��ʹ��Record������Ϊ���ݽṹ
	 * @throws IOException
	 */
	private Record readOneRecord(BufferedReader reader) throws IOException {
		String line = null;
		Record record = null;
		while ((line = reader.readLine()) != null) {
			// ԭ΢������ʹ��'\t'���ָ�ÿ�����ݵģ����Կ���������ȡÿ�������е��������
			String[] strArr = line.split("\t");
			//ֻ�����ʽ��ȷ��΢����ID��username��date��content 4�����ݶ��е�ֵ��
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
	 * ���main����
	 * 
	 * @param args
	 *            ΢�����ݵ�Ŀ¼·�� && Ҫ���ɵ��������ļ�Ŀ¼·��
	 */
	public static void main(String[] args) {
		Indexer indexer = new Indexer();
		try {
			// int num=indexer.index("E:\\weiboDataSearcher\\weibo_index",
			// "E:\\weiboDataSearcher\\weibo_data.txt"); ע�����ڲ���
			int num = indexer.index(args[0], args[1]);
			System.out.println("��������" + num + "ƪ΢��");
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
}
