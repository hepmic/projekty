#include <cstdlib>
#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>
#include <ctime>
#include <iomanip>
#include <string>

using namespace std;

string AllowedChars = " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

class selection
{
public:
  static int fitness(string candidate)
  {
    assert(target.length() == candidate.length());
    int FitnessValue = 0;
    for (int i = 0; i < target.length(); ++i)
    {
      int target_pos = AllowedChars.find(target[i]);
      int candidate_pos = AllowedChars.find(candidate[i]);
      int difference = abs(target_pos - candidate_pos);
      FitnessValue -= min(difference, int(AllowedChars.length()) - difference);
    }
    return FitnessValue;
  }
  static int target_length() { return target.length(); }
private:
  static string target;        //cel jest ukryty
};
string selection::target = "Chromosome representation";

bool less_fit(string const& s1, string const& s2)
{
  return selection::fitness(s1) < selection::fitness(s2);
}
void move_char(char& c, int distance)
{
  while (distance < 0)
    distance += AllowedChars.length();
  int char_pos = AllowedChars.find(c);
  c = AllowedChars[(char_pos + distance) % AllowedChars.length()];
}
string mutate(string parent, double MutationRate)
{
  for (int i = 0; i < parent.length(); ++i)
    if (rand()/(RAND_MAX + 1.0) < MutationRate)
    {
      int distance = rand() % 3 + 1;
      if(rand()%2 == 0)
        move_char(parent[i], distance);
      else
        move_char(parent[i], -distance);
    }
  return parent;
}
string mutate_krz(string parent_1, string parent_2){
	string result = parent_1;
	int sum1 = (-1) * selection::fitness(parent_1), sum2 = (-1) * selection::fitness(parent_2);
	double sum = sum1 / (sum1 + sum2);
	for(int i = 0; i < parent_1.length(); i++)
		{
			if(rand()/(RAND_MAX + 1.0) < sum)
				result[i] = parent_2[i];
		}
	return result;
}

int main()
{
  int const C = 250;
  srand(time(0));
  string parent;
  for (int i = 0; i < selection::target_length(); ++i)
  {
    parent += AllowedChars[rand() % AllowedChars.length()];      //losowanie poczatkowego stringa
  }
  int const InitialFitness = selection::fitness(parent);
	string parent_old[5] = {parent, parent, parent, parent, parent};
  for(int fitness = InitialFitness, j = 1;
      fitness < 0;
      fitness = selection::fitness(parent), j++)
  {
    cout << "Generacja nr:" << j << "   \t" << parent << ": " << fitness << "\n";
    const double MutationRate = 0.04 + (0.99*fitness)/InitialFitness;
    vector<string> childs;
    childs.reserve(C + 50);
    childs.push_back(parent);
    for (int i = 0; i < C; ++i)
      childs.push_back(mutate(parent, MutationRate));
      
    for(int i = 0; i < 5; i++)
		childs.push_back(parent_old[i]);
	for(int k = 0; k < 5; k++)
		for(int l = k + 1; l < 5; l++)
			childs.push_back(mutate_krz(parent_old[k], parent_old[l]));
			
    parent = *max_element(childs.begin(), childs.end(), less_fit);
    for(int i = 0; i < 5; i++)
		parent_old[i] = childs[i];
  }
  cout << "Wynik: " << parent << "\n";
}
