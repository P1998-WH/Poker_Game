#include <iostream>
#include <cstdlib>
#include <random>
#include <vector>
#include <string>
#include <set>
#include <utility>
#include <map>
#include <type_traits>

using namespace std;

enum HandRank
{
	NADA,
	HIGH_CARD,
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

	bool operator<(const Card& other) const {
        if (card != other.card) 
            return card < other.card;
        return suit < other.suit;
    }
};

struct Player
{
	string name;
	Card hand[2];
	HandRank rank = NADA;
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

	template<typename T>
	HandRank check_Same(const vector<T>& comb, Player& p)
	{
		bool is_String = false;
		map<T, int> count_map;
		if(std::is_same<T, string>::value)
		{
			is_String = true;
		}
		for(auto &c : comb)
		{
			count_map[c]++;
		}
		for(const auto &a : count_map)
		{
			if(is_String && a.second == 5)
			{
				p.rank = FLUSH;
				return p.rank;
			}
			else
			{
			if(a.second == 4)
				{
					p.rank = FOUR_OF_A_KIND;
					return p.rank;
				}
			else if(a.second == 3)
				{
					p.rank = THREE_OF_A_KIND;
					return p.rank;
				}
			else if(a.second == 2 && count_map.size() == 4)
			{
				p.rank = TWO_PAIR;
				return p.rank;
			}
			else if(a.second == 2 && count_map.size() == 6)
			{
				p.rank = ONE_PAIR;
				return p.rank;
			}
			else
			{
				cout << "No repeatition" << endl;
				return NADA;
			}
		}
		}
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

	string winner(Player &p)
	{
		return p.name;
	}
	void evaluateHand(Player &p, const vector<Card>& tCards)
	{
		vector<Card> combined;

		combined.push_back(p.hand[0]);
		combined.push_back(p.hand[1]);
		combined.insert(combined.end(), tCards.begin(), tCards.end());

		for(auto &c : combined)
		{
			cout << c.card << c.suit <<  " | ";
		}
		cout << "\n";

		//FLUSH && STRAIGHT
		check_Same(combined, p);
		cout << "Rank : " << p.rank << endl;
		cout << winner(p) << endl;
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

