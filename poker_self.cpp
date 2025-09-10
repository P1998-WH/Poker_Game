#include <iostream>
#include <cstdlib>
#include <random>
#include <vector>
#include <string>
#include <set>
#include <utility>
#include <map>

using namespace std;

enum HandRank
{
	HIGH_CARD = 1,
	ONE_PAIR,
	TWO_PAIR,
	THREE_OF_A_KIND,
	STRAIGHT,
	FLUSH,
	FULL_HOUSE,
	FOUR_OF_A_KIND,
	STRAIGHT_FLUSH,
	ROYAL_FLUSH
};

struct Card
{
	int card;
	string suit;
};

struct Player
{
	string name;
	Card hand[2];
	HandRank rank;
	vector<int> tiebreaker;
};

random_device rd;
mt19937 gen(rd());
uniform_int_distribution<int> dist_card(1,13);
uniform_int_distribution<int> dist_suite(0,3);
string suite_list[4] = {"D", "H", "C", "S"};
set<pair<int,int>> used_card;

class poker
{
public:

	Card unq_Cards()
	{
		int uq_card, uq_suite;
		do
		{
			uq_card = dist_card(gen);
			uq_suite = dist_suite(gen);
		}
		while(used_card.count({uq_card, uq_suite}));

		used_card.insert({uq_card, uq_suite});

		return {uq_card, suite_list[uq_suite]};
	}

	poker()
	{
		gen.seed(rd());
	}

	vector<Player> createPlayer(int numPlayer)
	{
		vector<Player> p_Info;
		p_Info.reserve(numPlayer);
		for(int i = 0; i < numPlayer; i++)
		{
			Player p;
			cout << "enter name of player : " << (i + 1) << " \n";
			getline(cin, p.name);

			for(int j = 0; j < 2; j++)
			{
				p.hand[j] = unq_Cards();

			}
			p_Info.push_back(p);
		}
		return p_Info;
	}

	vector<Card> deckCards()
	{
		vector<Card> dk_Cards;
		for(int i =0; i < 5; i++)
		{
			dk_Cards.push_back(unq_Cards());

		}
		for(auto &c : dk_Cards)
			cout << c.card << c.suit << " ";
		cout << "\n";
		return dk_Cards;
	}

	int check_repeatition(int target, const vector<Card> comb)
	{
		int repeat = 0;
		for (size_t i = 2; i < comb.size(); i++)
		{
		  //  cout << "check_repeatition target "<< target << "\n";
			if (comb[i].card == target)
				repeat++;
		}
		return repeat;
	}

	void showPlayer(const vector<Player>& player)
	{
		for(auto &p : player)
		{
			cout << "Name: " << p.name ;

			for(int j = 0; j < player.size(); j++)
			{
				cout << " | Suite: " <<  p.hand[j].suit << " | Card : " << p.hand[j].card << endl;
			}
		}
	}
	
	int s_case(int num, Player& p)
	{
	   switch(num) {
	   case 0:
	       cout << "NULL repeatition" << endl;
	       break;
	   case 1:
	        cout << "ONE_PAIR " << ONE_PAIR << endl;
	        p.rank = ONE_PAIR;
	        break;
	   case 2:
	        cout << "TWO_PAIR " << TWO_PAIR << endl;
	        p.rank = TWO_PAIR;
	        break;
	   case 3:
	        cout << "THREE_OF_A_KIND " << THREE_OF_A_KIND << endl;
	        p.rank = THREE_OF_A_KIND;
	        break;
	   case 4:
	        cout << "FOUR_OF_A_KIND " << FOUR_OF_A_KIND << endl;
	        p.rank = FOUR_OF_A_KIND;
	        break;
	   }
	   return p.rank;
	}

	void evaluateHand(Player &p, const vector<Card>& tCards)
	{
		vector<Card> combined;
		bool alreadyPair = false;
		int target;
		combined.push_back(p.hand[0]);
		combined.push_back(p.hand[1]);
		combined.insert(combined.end(), tCards.begin(), tCards.end());

		for(auto &c : combined)
		{
			cout << c.card << c.suit <<  " | ";
		}
		cout << "\n";

		if(p.hand[0].card == p.hand[1].card)
		{
			p.rank = ONE_PAIR;
			cout << "Player has been dealt ONE_PAIR: " << ONE_PAIR
			     << " | "<< p.hand[0].card << p.hand[0].suit << " "
			     << p.hand[1].card << p.hand[1].suit << endl;
			alreadyPair = true;
		}


		int tot_Rep;
		int repeat = 0;
		//check for pair, 3 of a kind and 4 of a kind
		if(!alreadyPair)
		{
			for(int i = 0 ; i < 2; i++)
			{
				target = p.hand[i].card;
				// cout << "target "<< target << "\n";
				repeat = check_repeatition(target, combined);
				// cout << "FALSE "<< alreadyPair << "\n";
			}
// 			cout << "repeat "<< repeat << "\n";
			int score = s_case(repeat, p);
// 			cout << "Score: " << score << endl;
			repeat = 0;
		}
		else
		{
			target = p.hand[0].card;
			repeat = check_repeatition(target, combined);
// 			cout << "TRUE" << alreadyPair << "\n";
			repeat = repeat + 1;
// 			cout << "repeat "<< repeat << "\n";
			int score = s_case(repeat, p);
// 			cout << "Score: " << score << endl;
		}

	

	}

};


int main()
{
	poker p;
	int num;
	cout << "Total Players : ";
	cin >> num;
	cin.ignore();

	auto players = p.createPlayer(num);

	p.showPlayer(players);
	auto deck = p.deckCards();

	for(auto &z : players)
		p.evaluateHand(z, deck);

	return 0;
}