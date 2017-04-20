#include "GameHandler.h"

GameHandler::GameHandler()
{
	welcome();
	initGame();
}

GameHandler::~GameHandler()
{
	clearGame();
	cout<<"游戏结束！"<<endl;
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

	cout<<"初始化游戏..."<<endl;

	cout<<"输入玩家个数（默认为1）：";
	cin>>_numberOfPlayers;

	cout<<"输入初始赌金（默认为100）：";
	cin>>_initBalance;

	cout<<"输入爆牌/输赢规则（默认为21点）：";
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

	cout<<"游戏初始化成功..."<<endl<<endl;
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
			cout<<"输入玩家"<<i+1<<"的昵称：";
			cin>>name;
			players[i]->setName(name);
			cout<<"玩家"<<i+1<<':'<<players[i]->getName()<<"进入游戏！"<<endl<<endl;
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

	cout<<"    "<<"最大点数和为："<<cards->getTotalValues()<<endl;

	delete tmpCards;
}

void GameHandler::play()
{
	char response;
	do 
	{
		loopPlay();
		cout<<"是否继续开始下一轮？(Y/N)"<<endl;
		cin>>response;
	} while (response=='Y'|| response=='y');
}

void GameHandler::chooseDeck()
{
	int i,j;
	int n=0;
	cout<<"牌组选项："<<endl;
	cout<<"1. 52张牌"<<endl;
	cout<<"2. 52张牌抽掉2张"<<endl;
	cout<<"3. 去掉全部黑桃牌"<<endl;

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
	//显示赌金，并进行第一轮下注
	for (j = 0; j< numberOfPlayers; ++j)
	{
		cout<<"玩家"<<j<<"："<<players[j]->getName()<<"目前有赌金："<<players[j]->getBalance()<<endl;
		cout<<"请下注：";
		cin>>bets;
		betList[j]=bets;
		cout<<"玩家"<<j<<"："<<players[j]->getName()<<"下注金额为"<<betList[j]<<endl;
		players[j]->makeBet(betList[j]);
	}
	cout<<endl;
}

void GameHandler::initHands()
{
	int i,j;

	cout<<"第一轮发牌，每人初始先发两张牌..."<<endl;
	//将最初的两张牌给每一个人
	for (i = 0; i < 2; ++i)
	{
		for (j = 0; j< numberOfPlayers; ++j)
			decks[j]->dealACard(*players[j]);
		deck->dealACard(*dealer);
	}

	dealer->flipFirstCard();//隐藏庄家的第一张牌

	//显示专家手牌
	cout<<"庄家初始手牌：";
	showCards(dealer);
	cout<<endl;

	for (j = 0; j< numberOfPlayers; ++j)
		//显示每个玩家的初始的两张手牌
	{
		cout<<"玩家"<<j<<"："<<players[j]->getName()<<"手牌：";
		showCards(players[j]);
	}
	cout<<endl;
}

void GameHandler::clearAllHands()
{
	//删除每个人的牌
	for (int j = 0; j< numberOfPlayers; ++j)
		players[j]->clear();
	dealer->clear();
}

void GameHandler::playerTurn()
{
	double bets;
	char response;
	int j;

	cout<<"玩家回合："<<endl;

	for (j=0; j<numberOfPlayers;++j)
	{
		cout<<"玩家"<<j<<"："<<players[j]->getName()<<"是否继续要牌？(Y/N)";
		cin>>response;
		if (response=='Y' || response=='y') players[j]->request(true);
		else players[j]->request(false);

		while (!players[j]->isBusted() && players[j]->isHitting())
		{
			//在下的过程中可以增加赌金
			cout<<"是否加注？(Y/N)";
			cin>>response;
			if (response=='Y' || response=='y')
			{
				cout<<"输入增加的赌金：";
				cin>>bets;
				betList[j]+=bets;
				cout<<"赌金增加至"<<betList[j]<<endl;
				players[j]->makeBet(bets);
			}

			decks[j]->dealACard(*players[j]);

			cout<<"玩家"<<j<<"："<<players[j]->getName()<<"现有手牌：";
			showCards(players[j]);

			if (players[j]->isBusted()) 
			{
				cout<<"爆牌!!!"<<endl;
				break;
			}

			cout<<"玩家"<<j<<"："<<players[j]->getName()<<"是否继续要牌？(Y/N)";
			cin>>response;
			if (response=='Y' || response=='y') players[j]->request(true);
			else players[j]->request(false);
		}
	}

	cout<<endl;
}

void GameHandler::dealerTurn()
{
	cout<<"庄家回合："<<endl;
	dealer->flipFirstCard();      //显示庄家的第一张牌
	cout<<"庄家初始手牌：";
	showCards(dealer);
	cout<<endl;

	//向庄家发额外的牌
	while (!dealer->isBusted() && dealer->isHitting())
		deck->dealACard(*dealer);
	cout<<"庄家手牌：";
	showCards(dealer);
	if (dealer->isBusted()) cout<<"爆牌!!!"<<endl;
	cout<<endl;
}

void GameHandler::againstDealer()
{
	int j;
	if (dealer->isBusted())
	{   
		//在玩的每个未爆牌的玩家都胜利
		for (j=0; j<numberOfPlayers;++j)
		{
			if (!players[j]->isBusted()) 
			{
				cout<<"玩家"<<j<<"："<<players[j]->getName()<<" 赢了!!获得赌金"<<2*betList[j]<<endl;
				players[j]->getBet(2*betList[j]);
			}
			else cout<<"玩家"<<j<<"："<<players[j]->getName()<<" 输了!!失去赌金"<<betList[j]<<endl;
		}
	}
	else
	{      //在玩的各个和庄家比较
		for (j=0; j<numberOfPlayers;++j)   
			if ( !players[j]->isBusted())
			{
				if (players[j]->getTotalValues() > dealer->getTotalValues())
				{
					cout<<"玩家"<<j<<"："<<players[j]->getName()<<" 赢了!!获得赌金"<<2*betList[j]<<endl;
					players[j]->getBet(2*betList[j]);
				}
				else if (players[j]->getTotalValues() < dealer->getTotalValues())
				{
					cout<<"玩家"<<j<<"："<<players[j]->getName()<<" 输了!!失去赌金"<<betList[j]<<endl;
				}
				else
				{
					cout<<"玩家"<<j<<"："<<players[j]->getName()<<" 平局!!归还赌金"<<betList[j]<<endl;
					players[j]->getBet(betList[j]);
				}
			}
			else cout<<"玩家"<<j<<"："<<players[j]->getName()<<" 输了!!失去赌金"<<betList[j]<<endl;
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
		//在玩的每个未爆牌的玩家进行比较
		for (j=0; j<numberOfPlayers;++j)
			if (playersValue[j]<=burstValue)
			{
				maxPoint=playersValue[j];
				i=j;
				break;
			}

		//全部爆牌
		if (maxPoint==0)
		{
			cout<<"庄家和所有玩家均爆牌，平局！归还赌金"<<endl;
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

			//胜利者拿回自己的赌金，且平分失败者的赌金
			for (j=0;j<i;j++)
				bets-=betList[winList[j]];
			bets=bets/i;

			for (j=0;j<i;j++)
			{
				cout<<"玩家"<<winList[j]<<"："<<players[winList[j]]->getName()<<" 赢了!!获得赌金"<<bets+betList[winList[j]]<<endl;
				players[winList[j]]->getBet(bets+betList[winList[j]]);
			}
			if (i<numberOfPlayers) cout<<"其他玩家输了，失去赌金"<<endl;
		}
		cout<<endl;
	}
	else
	{
		//在玩的每个未爆牌且点数大于庄家的玩家进行比较
		for (j=0; j<numberOfPlayers;++j)
			if (playersValue[j]<=burstValue)
			{
				maxPoint=playersValue[j];
				i=j;
				break;
			}
		if (maxPoint==0)
			//除了庄家外其他人都爆牌
			cout<<"所有玩家均爆牌，庄家获胜，玩家失去赌金"<<endl;
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

			//胜利者拿回自己的赌金，且平分失败者的赌金
			for (j=0;j<i;j++)
				bets-=betList[winList[j]];
			bets=bets/i;

			for (j=0;j<i;j++)
			{
				if (maxPoint>dealerValue)
				{
					cout<<"玩家"<<winList[j]<<"："<<players[winList[j]]->getName()<<" 赢了!!获得赌金"<<bets+betList[winList[j]]<<endl;
					players[winList[j]]->getBet(bets+betList[winList[j]]);
				}
				else
				{
					cout<<"玩家"<<winList[j]<<"："<<players[winList[j]]->getName()<<" 平局!!归还赌金"<<betList[winList[j]]<<endl;
					players[winList[j]]->getBet(betList[winList[j]]);
				}
			}
			if (i<numberOfPlayers) cout<<"其他玩家输了，失去赌金"<<endl;
		}		
		cout<<endl;
	}

	delete[] playersValue; 
	delete[] winList;	
}

//核心逻辑
void GameHandler::loopPlay()
{
	int i;
	if (numberOfPlayers>1) cout<<"请选择游戏模式："<<endl;
	cout<<"1. 每个玩家只和庄家比较点数"<<endl;
	cout<<"2. 除了和庄家比较点数，玩家之间也要比较点数，赢的人平分赌注"<<endl;
	cin>>i;

	//选择牌堆类型
	chooseDeck();
	//未开局前下注
	makeFirstBet();
	//初始发两张牌，并显示手牌
	initHands();
	//玩家回合，分别询问每个玩家是否要牌
	playerTurn();
	//庄家回合
	dealerTurn();

	//游戏模式
	if (i==1) againstDealer();
	else againstPlayers();

	//清除手牌
	clearAllHands();
}