#include "GameHandler.h"

GameHandler::GameHandler()
{
	welcome();
	initGame();
}

GameHandler::~GameHandler()
{
	clearGame();
	cout<<"��Ϸ������"<<endl;
	cin.get();
	cin.get();
}

void GameHandler::welcome()
{
	cout<<"Welcome to BlackJack Game"<<endl;
	cout<<"Designed by ZhangXinqian"<<endl<<endl;
}

void GameHandler::initGame()
{
	int _numberOfPlayers;
	int _burstValue;
	double _initBalance;

	cout<<"��ʼ����Ϸ..."<<endl;

	cout<<"������Ҹ�����Ĭ��Ϊ1����";
	cin>>_numberOfPlayers;

	cout<<"�����ʼ�Ľ�Ĭ��Ϊ100����";
	cin>>_initBalance;

	cout<<"���뱬��/��Ӯ����Ĭ��Ϊ21�㣩��";
	cin>>_burstValue;

	int i;

	dealer=new Dealer(_burstValue);
	deck=new Deck();

	decks=new Deck *[_numberOfPlayers];
	players=new Player*[_numberOfPlayers];
	for (i=0;i<_numberOfPlayers;i++)
	{
		decks[i]=new Deck();
		players[i]=new Player(_initBalance,_burstValue);
	}

	betList=new double[_numberOfPlayers];

	numberOfPlayers=_numberOfPlayers;
	initBalance=_initBalance;
	burstValue=_burstValue;

	setPlayersName();

	cout<<"��Ϸ��ʼ���ɹ�..."<<endl<<endl;
}

void GameHandler::clearGame()
{
	delete deck;
	delete dealer;
	delete[] betList;
	for (int i=0;i<numberOfPlayers;i++)
		delete players[i];
	delete players;
}

void GameHandler::setPlayersName()
{
	if (numberOfPlayers>0)
	{
		string name;
		for (int i=0;i<numberOfPlayers;i++)
		{
			players[i]->setId(i);
			cout<<"�������"<<i+1<<"���ǳƣ�";
			cin>>name;
			players[i]->setName(name);
			cout<<"���"<<i+1<<':'<<players[i]->getName()<<"������Ϸ��"<<endl<<endl;
		}
	}
}

void GameHandler::showACard(Card *card)
{
	if (card->getState())
	{
		string result="";
		switch(card->getSuit())
		{
		case SPADE:
			result+=char(6);
			break;
		case CLUB:
			result+=char(5);
			break;
		case HEART:
			result+=char(3);
			break;
		case DIAMOND:
			result+=char(4);
			break;
		default:
			break;
		}
		switch (card->getFace())
		{
		case ACE:
			result+='A';
			break;
		case TWO: 
		case THREE: 
		case FOUR: 
		case FIVE: 
		case SIX: 
		case SEVEN:
		case EIGHT:
		case NINE:
			result+=char(card->getFace()-ACE+'1');
			break;
		case TEN:
			result+="10";
		case JACK:
			result+='J';
			break;
		case QUEEN:
			result+='Q';
			break;
		case KING:
			result+='K';
			break;
		default:
			break;
		}
		cout<<' '<<result;
	}
	else cout<<" ??";
}

void GameHandler::showCards(Cards *cards)
{
	list<Card*>* tmpCards=cards->getCards();

	list<Card*>::iterator iter;
	Card *card;

	for (iter=tmpCards->begin();iter!=tmpCards->end();++iter)
	{
		card=*iter;
		showACard(card);
	}

	cout<<"    "<<"��������Ϊ��"<<cards->getTotalValues()<<endl;

	delete tmpCards;
}

void GameHandler::play()
{
	char response;
	do 
	{
		loopPlay();
		cout<<"�Ƿ������ʼ��һ�֣�(Y/N)"<<endl;
		cin>>response;
	} while (response=='Y'|| response=='y');
}

void GameHandler::chooseDeck()
{
	int i,j;
	int n=0;
	cout<<"����ѡ�"<<endl;
	cout<<"1. 52����"<<endl;
	cout<<"2. 52���Ƴ��2��"<<endl;
	cout<<"3. ȥ��ȫ��������"<<endl;

	cin>>i;

	deck->createDeck();
	deck->shuffle(n++);
	for (j=0;j<numberOfPlayers;j++)
	{
		decks[j]->createDeck();
		decks[j]->shuffle(n++);
	}

	if (i==2)
	{
		for (j=0;j<numberOfPlayers;j++)
			decks[j]->removeCards(2);
		deck->removeCards(2);
	}
	else if (i==3)
	{
		for (j=0;j<numberOfPlayers;j++)
			decks[j]->removeCards(SPADE);
		deck->removeCards(SPADE);
	}
}

void GameHandler::makeFirstBet()
{
	int j;
	double bets;
	//��ʾ�Ľ𣬲����е�һ����ע
	for (j = 0; j< numberOfPlayers; ++j)
	{
		cout<<"���"<<j<<"��"<<players[j]->getName()<<"Ŀǰ�жĽ�"<<players[j]->getBalance()<<endl;
		cout<<"����ע��";
		cin>>bets;
		betList[j]=bets;
		cout<<"���"<<j<<"��"<<players[j]->getName()<<"��ע���Ϊ"<<betList[j]<<endl;
		players[j]->makeBet(betList[j]);
	}
	cout<<endl;
}

void GameHandler::initHands()
{
	int i,j;

	cout<<"��һ�ַ��ƣ�ÿ�˳�ʼ�ȷ�������..."<<endl;
	//������������Ƹ�ÿһ����
	for (i = 0; i < 2; ++i)
	{
		for (j = 0; j< numberOfPlayers; ++j)
			decks[j]->dealACard(*players[j]);
		deck->dealACard(*dealer);
	}

	dealer->flipFirstCard();//����ׯ�ҵĵ�һ����

	//��ʾר������
	cout<<"ׯ�ҳ�ʼ���ƣ�";
	showCards(dealer);
	cout<<endl;

	for (j = 0; j< numberOfPlayers; ++j)
		//��ʾÿ����ҵĳ�ʼ����������
	{
		cout<<"���"<<j<<"��"<<players[j]->getName()<<"���ƣ�";
		showCards(players[j]);
	}
	cout<<endl;
}

void GameHandler::clearAllHands()
{
	//ɾ��ÿ���˵���
	for (int j = 0; j< numberOfPlayers; ++j)
		players[j]->clear();
	dealer->clear();
}

void GameHandler::playerTurn()
{
	double bets;
	char response;
	int j;

	cout<<"��һغϣ�"<<endl;

	for (j=0; j<numberOfPlayers;++j)
	{
		cout<<"���"<<j<<"��"<<players[j]->getName()<<"�Ƿ����Ҫ�ƣ�(Y/N)";
		cin>>response;
		if (response=='Y' || response=='y') players[j]->request(true);
		else players[j]->request(false);

		while (!players[j]->isBusted() && players[j]->isHitting())
		{
			//���µĹ����п������ӶĽ�
			cout<<"�Ƿ��ע��(Y/N)";
			cin>>response;
			if (response=='Y' || response=='y')
			{
				cout<<"�������ӵĶĽ�";
				cin>>bets;
				betList[j]+=bets;
				cout<<"�Ľ�������"<<betList[j]<<endl;
				players[j]->makeBet(bets);
			}

			decks[j]->dealACard(*players[j]);

			cout<<"���"<<j<<"��"<<players[j]->getName()<<"�������ƣ�";
			showCards(players[j]);

			if (players[j]->isBusted()) 
			{
				cout<<"����!!!"<<endl;
				break;
			}

			cout<<"���"<<j<<"��"<<players[j]->getName()<<"�Ƿ����Ҫ�ƣ�(Y/N)";
			cin>>response;
			if (response=='Y' || response=='y') players[j]->request(true);
			else players[j]->request(false);
		}
	}

	cout<<endl;
}

void GameHandler::dealerTurn()
{
	cout<<"ׯ�һغϣ�"<<endl;
	dealer->flipFirstCard();      //��ʾׯ�ҵĵ�һ����
	cout<<"ׯ�ҳ�ʼ���ƣ�";
	showCards(dealer);
	cout<<endl;

	//��ׯ�ҷ��������
	while (!dealer->isBusted() && dealer->isHitting())
		deck->dealACard(*dealer);
	cout<<"ׯ�����ƣ�";
	showCards(dealer);
	if (dealer->isBusted()) cout<<"����!!!"<<endl;
	cout<<endl;
}

void GameHandler::againstDealer()
{
	int j;
	if (dealer->isBusted())
	{   
		//�����ÿ��δ���Ƶ���Ҷ�ʤ��
		for (j=0; j<numberOfPlayers;++j)
		{
			if (!players[j]->isBusted()) 
			{
				cout<<"���"<<j<<"��"<<players[j]->getName()<<" Ӯ��!!��öĽ�"<<2*betList[j]<<endl;
				players[j]->getBet(2*betList[j]);
			}
			else cout<<"���"<<j<<"��"<<players[j]->getName()<<" ����!!ʧȥ�Ľ�"<<betList[j]<<endl;
		}
	}
	else
	{      //����ĸ�����ׯ�ұȽ�
		for (j=0; j<numberOfPlayers;++j)   
			if ( !players[j]->isBusted())
			{
				if (players[j]->getTotalValues() > dealer->getTotalValues())
				{
					cout<<"���"<<j<<"��"<<players[j]->getName()<<" Ӯ��!!��öĽ�"<<2*betList[j]<<endl;
					players[j]->getBet(2*betList[j]);
				}
				else if (players[j]->getTotalValues() < dealer->getTotalValues())
				{
					cout<<"���"<<j<<"��"<<players[j]->getName()<<" ����!!ʧȥ�Ľ�"<<betList[j]<<endl;
				}
				else
				{
					cout<<"���"<<j<<"��"<<players[j]->getName()<<" ƽ��!!�黹�Ľ�"<<betList[j]<<endl;
					players[j]->getBet(betList[j]);
				}
			}
			else cout<<"���"<<j<<"��"<<players[j]->getName()<<" ����!!ʧȥ�Ľ�"<<betList[j]<<endl;
	}
}

void GameHandler::againstPlayers()
{
	int i,j;
	int dealerValue=dealer->getTotalValues();
	int *playersValue=new int[numberOfPlayers];
	int *winList=new int[numberOfPlayers];
	int maxPoint=0;
	for (j=0;j<numberOfPlayers;++j)
		playersValue[j]=players[j]->getTotalValues();

	if (dealer->isBusted())
	{   
		//�����ÿ��δ���Ƶ���ҽ��бȽ�
		for (j=0; j<numberOfPlayers;++j)
			if (playersValue[j]<=burstValue)
			{
				maxPoint=playersValue[j];
				i=j;
				break;
			}

		//ȫ������
		if (maxPoint==0)
		{
			cout<<"ׯ�Һ�������Ҿ����ƣ�ƽ�֣��黹�Ľ�"<<endl;
			for (j=0;j<numberOfPlayers;j++)
				players[j]->getBet(betList[j]);
		}
		else
		{
			for (j=i; j<numberOfPlayers;++j)
				if (maxPoint<playersValue[j] && playersValue[j]<=burstValue) maxPoint=playersValue[j];
			i=0;
			double bets=0;
			for (j=0; j<numberOfPlayers;++j)
			{
				bets+=betList[j];
				if (playersValue[j]==maxPoint) winList[i++]=j;
			}

			//ʤ�����û��Լ��ĶĽ���ƽ��ʧ���ߵĶĽ�
			for (j=0;j<i;j++)
				bets-=betList[winList[j]];
			bets=bets/i;

			for (j=0;j<i;j++)
			{
				cout<<"���"<<winList[j]<<"��"<<players[winList[j]]->getName()<<" Ӯ��!!��öĽ�"<<bets+betList[winList[j]]<<endl;
				players[winList[j]]->getBet(bets+betList[winList[j]]);
			}
			if (i<numberOfPlayers) cout<<"����������ˣ�ʧȥ�Ľ�"<<endl;
		}
		cout<<endl;
	}
	else
	{
		//�����ÿ��δ�����ҵ�������ׯ�ҵ���ҽ��бȽ�
		for (j=0; j<numberOfPlayers;++j)
			if (playersValue[j]<=burstValue)
			{
				maxPoint=playersValue[j];
				i=j;
				break;
			}
		if (maxPoint==0)
			//����ׯ���������˶�����
			cout<<"������Ҿ����ƣ�ׯ�һ�ʤ�����ʧȥ�Ľ�"<<endl;
		else
		{
			for (j=i; j<numberOfPlayers;++j)
				if (maxPoint<playersValue[j] && playersValue[j]<=burstValue && playersValue[j]>=dealerValue) 
					maxPoint=playersValue[j];
			i=0;
			double bets=0;
			for (j=0; j<numberOfPlayers;++j)
			{
				bets+=betList[j];
				if (playersValue[j]==maxPoint) winList[i++]=j;
			}

			//ʤ�����û��Լ��ĶĽ���ƽ��ʧ���ߵĶĽ�
			for (j=0;j<i;j++)
				bets-=betList[winList[j]];
			bets=bets/i;

			for (j=0;j<i;j++)
			{
				if (maxPoint>dealerValue)
				{
					cout<<"���"<<winList[j]<<"��"<<players[winList[j]]->getName()<<" Ӯ��!!��öĽ�"<<bets+betList[winList[j]]<<endl;
					players[winList[j]]->getBet(bets+betList[winList[j]]);
				}
				else
				{
					cout<<"���"<<winList[j]<<"��"<<players[winList[j]]->getName()<<" ƽ��!!�黹�Ľ�"<<betList[winList[j]]<<endl;
					players[winList[j]]->getBet(betList[winList[j]]);
				}
			}
			if (i<numberOfPlayers) cout<<"����������ˣ�ʧȥ�Ľ�"<<endl;
		}		
		cout<<endl;
	}

	delete[] playersValue; 
	delete[] winList;	
}

//�����߼�
void GameHandler::loopPlay()
{
	int i;
	if (numberOfPlayers>1) cout<<"��ѡ����Ϸģʽ��"<<endl;
	cout<<"1. ÿ�����ֻ��ׯ�ұȽϵ���"<<endl;
	cout<<"2. ���˺�ׯ�ұȽϵ��������֮��ҲҪ�Ƚϵ�����Ӯ����ƽ�ֶ�ע"<<endl;
	cin>>i;

	//ѡ���ƶ�����
	chooseDeck();
	//δ����ǰ��ע
	makeFirstBet();
	//��ʼ�������ƣ�����ʾ����
	initHands();
	//��һغϣ��ֱ�ѯ��ÿ������Ƿ�Ҫ��
	playerTurn();
	//ׯ�һغ�
	dealerTurn();

	//��Ϸģʽ
	if (i==1) againstDealer();
	else againstPlayers();

	//�������
	clearAllHands();
}