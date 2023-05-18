#include<iostream>
#include<cmath>
#include<string>
#include<iomanip>
#include<unordered_map>
#define float double
using namespace std;
const string wuqi[3] = { "sword","bomb","arrow" };
const string mingzi[5] = { "dragon","ninja","iceman","lion","wolf" };
int redorder[5] = { 2,3,4,1,0 };
int blueorder[5] = { 3,0,1,2,4 };
const string yanse[2] = { "red","blue" };
int now;
void showtime()
{
	cout.fill('0');
	cout << setw(3) << now / 60 << ':' << setw(2) << now % 60 << ' ';
}
int initial_elements;
int numcity;
int arrow_power;
int loyalty_drop;
int time_limit;
int strength[5];//dragon 、ninja、iceman、lion、wolf 
int power[5];


class weapon
{
public:
	int firepower;
	string weapon_name;
	weapon() {}
	weapon(int a) { firepower = a; }
	virtual int afterfight() { return 0x3ffffff; }
	virtual void report(){}
};
class sword :public weapon
{
public:
	//sword武器的初始攻击力为拥有它的武士的攻击力的20%（去尾取整）。
	//但是sword每经过一次战斗(不论是主动攻击还是反击)，就会变钝，攻击力变为本次战斗前的80% (去尾取整)。
	//sword攻击力变为0时，视为武士失去了sword。如果武士降生时得到了一个初始攻击力为0的sword，则视为武士没有sword.
	sword(int a) :weapon(a) { weapon_name = "sword"; }
	int afterfight() { firepower = firepower * 4 / 5; return firepower; }
	void report()
	{
		cout << weapon_name << '(' << firepower << ')';
	}
};
class arrow :public weapon
{
public:
	//arrow有一个攻击力值R。如果下一步要走到的城市有敌人，那么拥有arrow的武士就会放箭攻击下一个城市的敌人
	//（不能攻击对方司令部里的敌人）而不被还击。arrow使敌人的生命值减少R，若减至小于等于0，则敌人被杀死。
	//arrow使用3次后即被耗尽，武士失去arrow。两个相邻的武士可能同时放箭把对方射死。
	int usedtimes;
	arrow(int a) :weapon(a) { usedtimes = 0; weapon_name = "arrow"; }
	int afterfight() { usedtimes += 1; return usedtimes; }
	void report()
	{
		cout << weapon_name << '(' << 3- usedtimes << ')';
	}
};
class bomb :public weapon
{
public:
	bomb(int a = 0) :weapon(0) { weapon_name = "bomb"; }
	void report() { cout << weapon_name; }
};


class warrior
{
public:
	int life, force, code, pos, color;
	bool has_weapon[3] = { 0 };
	string name;
	weapon* arms[3];

	warrior() {}
	warrior(int _life, int _force, int _code, int _pos, int _color, string _name)
	{
		life = _life; force = _force; code = _code; pos = _pos; color = _color; name = _name;
	}
	/*
	warrior(const warrior& s)
	{
		life = s.life; force = s.force; code = s.code; pos = s.pos; color = s.color; name = s.name;
		for (int i = 0; i <= 2; i++)
		{
			has_weapon[i] = s.has_weapon[i];
			if (has_weapon[i])
			{
				arms[i]=new
			}
		}
	}*/
	virtual bool isable_run() { return 0; }
	virtual void showrun(){}

	void common_born() { showtime(); cout << yanse[color] << ' ' << name << ' ' << code << " born" << endl; }
	virtual void showborn() {}

	void common_march()
	{
		if (yanse[color] == "red") { pos += 1; }
		else { pos -= 1; }
		if (name == "iceman")
		{
			if(color==0)
			{
				if (pos >= 1 and pos <= numcity+1 and ((pos - color) & 1) == 0)
				{
					force += 20;
					if (life >= 11) { life -= 9; }
					else { life = 1; }
				}
			}
			else
			{
				if (pos >= 1 and pos <= numcity and ((numcity+1-pos) & 1) == 0)
				{
					force += 20;
					if (life >= 11) { life -= 9; }
					else { life = 1; }
				}
			}
		}
	}
	void showmarch()
	{
		showtime();
		cout << yanse[color] << ' ' << name << ' ' << code << " marched to city " << pos;
		cout << " with " << life << " elements and force " << force << endl;
	}
	void show_reach_head()
	{
		showtime();
		cout << yanse[color] << ' ' << name << ' ' << code << " reached " << yanse[1 - color];
		cout << " headquarter with " << life << " elements and force " << force << endl;
	}
	void show_capture()
	{
		showtime();
		cout << yanse[1 - color] << " headquarter was taken" << endl;
	}

	void showshot() { showtime(); cout << yanse[color] << ' ' << name << ' ' << code << " shot" << endl; }
	void showshot_kill(warrior* t)
	{
		showtime();
		cout << yanse[color] << ' ' << name << ' ' << code << " shot and killed ";
		cout << yanse[t->color] << ' ' << t->name << ' ' << t->code << endl;
	}

	void showbomb(warrior*t)
	{
		showtime();
		cout << yanse[color] << ' ' << name << ' ' << code << " used a bomb and killed ";
		cout << yanse[t->color] << ' ' << t->name << ' ' << t->code << endl;
	}

	void attack(warrior* t)
	{
		if (has_weapon[0])
		{
			t->life -= (arms[0]->firepower + force);
			if (arms[0]->afterfight() == 0) { has_weapon[0] = 0; }
		}
		else { t->life -= force; }
	}
	void show_attack(warrior* t)
	{
		cout << yanse[color] << ' ' << name << ' ' << code << " attacked ";
		cout << yanse[t->color] << ' ' << t->name << ' ' << t->code;
		cout << " in city " << pos << " with " << life << " elements and force " << force << endl;
	}
	void simulate_attack(warrior* t)
	{
		if (has_weapon[0]) { t->life -= (arms[0]->firepower + force); }
		else { t->life -= force; }
	}
	void fight_back(warrior* t)
	{
		if (name == "ninja") { return; }
		if (has_weapon[0])
		{
			t->life -= (arms[0]->firepower + force / 2);
			if (arms[0]->afterfight() == 0) { has_weapon[0] = 0; }
		}
		else { t->life -= force / 2; }
	}
	void show_fight_back(warrior* t)
	{
		cout << yanse[color] << ' ' << name << ' ' << code << " fought back against ";
		cout << yanse[t->color] << ' ' << t->name << ' ' << t->code << " in city " << pos << endl;
	}
	void simulate_fight_back(warrior* t)
	{
		if (name == "ninja") { return; }
		if (has_weapon[0]) { t->life -= (arms[0]->firepower + force / 2); }
		else { t->life -= force / 2; }
	}
	void show_death()
	{
		cout << yanse[color] << ' ' << name << ' ' << code << " was killed in city " << pos << endl;
	}
	virtual void special_win() {}
	virtual void special_win(warrior* t) {}
	virtual void special_no_win() {}
	virtual void show_yeah() {}
	void show_earn_element(int k)
	{
		cout << yanse[color] << ' ' << name << ' ' << code << " earned ";
		cout << k << " elements for his headquarter" << endl;
	}

	void show_weapon()
	{
		int have = 0;
		for (int j = 2; j >= 0; j--)//交代武器情况时，次序依次是：arrow,bomb,sword。
		{
			if (has_weapon[j]) { have += 1; }
		}
		if (have)
		{
			showtime(); cout << yanse[color] << ' ' << name << ' ' << code << " has ";
			for (int j = 2; j >= 0; j--)//交代武器情况时，次序依次是：arrow,bomb,sword。
			{
				if (has_weapon[j] and have>=2)
				{
					arms[j]->report(); cout << ',';
					have -= 1;
				}
				else if (has_weapon[j] and have == 1)
				{
					arms[j]->report(); cout << endl;
				}
			}
		}
		else
		{
			showtime(); cout << yanse[color] << ' ' << name << ' ' << code << " has no weapon"<<endl;
		}
	}
};
class dragon :public warrior
{
public:
	float morale;//其值为它降生后其司令部剩余生命元的数量除以造dragon所需的生命元数量
	//dragon 在一次在它"主动"进攻的战斗结束后，如果还没有战死，而且士气值大于0.8，就会欢呼。
	//dragon每取得一次战斗的胜利(敌人被杀死)，士气就会增加0.2，每经历一次未能获胜的战斗，士气值就会减少0.2。
	//士气增减发生在欢呼之前。
	dragon(int _life, int _force, int _code, int _pos, int _color, int remain_elements) :
		warrior(_life, _force, _code, _pos, _color, "dragon")
	{
		morale = (float)remain_elements / (float)_life;
		if (wuqi[_code % 3] == "sword")
		{
			if (_force / 5 != 0) { has_weapon[0] = 1; arms[0] = new sword(_force / 5); }
		}
		else if (wuqi[_code % 3] == "bomb") { has_weapon[1] = 1; arms[1] = new bomb(0); }
		else { has_weapon[2] = 1; arms[2] = new arrow(arrow_power); }
	}

	void showborn()
	{
		common_born();
		cout << "Its morale is " << fixed << setprecision(2) << morale << endl;
	}
	void special_win() { morale += 0.2; }
	void special_no_win() { morale -= 0.2; }
	void show_yeah()
	{
		if (morale > 0.8) 
		{ 
			showtime(); 
			cout << yanse[color] << ' ' << name << ' ' << code << " yelled in city " << pos << endl; 
		}
	}
};
class ninja :public warrior
{
public:
	//编号为n的ninja降生时即获得编号为 n%3 和 (n+1)%3的武器,不会反击
	ninja(int _life, int _force, int _code, int _pos, int _color) :
		warrior(_life, _force, _code, _pos, _color, "ninja")
	{
		if (_code % 3 == 0)
		{
			has_weapon[1] = 1; arms[1] = new bomb(0);
			if (_force / 5 != 0) { has_weapon[0] = 1; arms[0] = new sword(_force / 5); }
		}
		if (_code % 3 == 1)
		{
			has_weapon[1] = 1; arms[1] = new bomb(0);
			has_weapon[2] = 1; arms[2] = new arrow(arrow_power);
		}
		if (_code % 3 == 2)
		{
			has_weapon[2] = 1; arms[2] = new arrow(arrow_power);
			if (_force / 5 != 0) { has_weapon[0] = 1; arms[0] = new sword(_force / 5); }
		}
	}
	void showborn() { common_born(); }
};
class iceman :public warrior
{
public:
	//iceman 每前进两步，在第2步完成的时候，生命值会减少9，攻击力会增加20。
	//color与所在城市奇偶性相同启动
	//但是若生命值减9后会小于等于0，则生命值不减9,而是变为1。即iceman不会因走多了而死
	iceman(int _life, int _force, int _code, int _pos, int _color) :
		warrior(_life, _force, _code, _pos, _color, "iceman")
	{
		if (wuqi[_code % 3] == "sword")
		{
			if (_force / 5 != 0) { has_weapon[0] = 1; arms[0] = new sword(_force / 5); }
		}
		else if (wuqi[_code % 3] == "bomb") { has_weapon[1] = 1; arms[1] = new bomb(0); }
		else { has_weapon[2] = 1; arms[2] = new arrow(arrow_power); }
	}

	void showborn() { common_born(); }
};
class lion :public warrior
{
public:
	int loyalty; //其初始值等于它降生之后其司令部剩余生命元的数目
	//每经过一场未能杀死敌人的战斗，忠诚度就降低K。忠诚度降至0或0以下，则该lion逃离战场,永远消失。
	//但是已经到达敌人司令部的lion不会逃跑。Lion在己方司令部可能逃跑。
	//lion 若是战死，则其战斗前的生命值就会转移到对手身上。所谓“战斗前”，就是每个小时的40分前的一瞬间。
	lion(int _life, int _force, int _code, int _pos, int _color, int remain_elements) :
		warrior(_life, _force, _code, _pos, _color, "lion") {
		loyalty = remain_elements;
	}

	void showborn()
	{
		common_born(); 
		cout << "Its loyalty is " << loyalty << endl;
	}
	void special_no_win() { loyalty -= loyalty_drop; }
	void showrun() { cout << yanse[color] << ' ' << name << ' ' << code << " ran away"<<endl; }
	bool isable_run()
	{
		return loyalty <= 0;
	}
};
class wolf :public warrior
{
public:
	//可以缴获武器
	wolf(int _life, int _force, int _code, int _pos, int _color) :
		warrior(_life, _force, _code, _pos, _color, "wolf") {}

	void showborn() { common_born(); }
	void special_win(warrior* t)
	{
		for (int j = 0; j <= 2; j++)
		{
			if (!has_weapon[j] and t->has_weapon[j])
			{
				arms[j] = t->arms[j];
				has_weapon[j] = 1;
			}
		}
	}
};


class city
{
public:
	int flagcolor;
	bool ishong = 0, islan = 0, isable = 0;//isable表示发生过战斗
	warrior* hong, * lan, * dead;//dead记录死者生前信息
	weapon* left_weapon[3];//用于缴获，及时delete
	int left_element;
	int redvictory = 0, bluevictory = 0;//连胜计数器
	city() { flagcolor = -1; left_element = 0; }//-1表示没有旗帜,0红1蓝
};

city chengshi[23];
//chengshi[0(numcity+1)]只考虑ishong(islan),hong,lan

class headquater
{
public:
	int element = initial_elements, enemy_num = 0;
};
class R :public headquater
{
public:
	int redpoint = 0;
	void make_warrior()
	{
		if (element >= strength[redorder[redpoint % 5]])
		{
			if (redorder[redpoint % 5] == 0)
			{
				chengshi[0].hong = new dragon(strength[0], power[0], redpoint + 1, 0, 0, element - strength[0]);
				chengshi[0].ishong = 1; chengshi[0].hong->showborn();
			}
			if (redorder[redpoint % 5] == 1)
			{
				chengshi[0].hong = new ninja(strength[1], power[1], redpoint + 1, 0, 0);
				chengshi[0].ishong = 1; chengshi[0].hong->showborn();
			}
			if (redorder[redpoint % 5] == 2)
			{
				chengshi[0].hong = new iceman(strength[2], power[2], redpoint + 1, 0, 0);
				chengshi[0].ishong = 1; chengshi[0].hong->showborn();
			}
			if (redorder[redpoint % 5] == 3)
			{
				chengshi[0].hong = new lion(strength[3], power[3], redpoint + 1, 0, 0, element - strength[3]);
				chengshi[0].ishong = 1; chengshi[0].hong->showborn();
			}
			if (redorder[redpoint % 5] == 4)
			{
				chengshi[0].hong = new wolf(strength[4], power[4], redpoint + 1, 0, 0);
				chengshi[0].ishong = 1; chengshi[0].hong->showborn();
			}
			element -= strength[redorder[redpoint % 5]];
			redpoint++;
		}
	}
};
class B :public headquater
{
public:
	int bluepoint = 0;
	void make_warrior()
	{
		if (element >= strength[blueorder[bluepoint % 5]])
		{
			if (blueorder[bluepoint % 5] == 0)
			{
				chengshi[numcity+1].lan = new dragon(strength[0], power[0], bluepoint + 1, numcity + 1, 1, element - strength[0]);
				chengshi[numcity + 1].islan = 1; chengshi[numcity + 1].lan->showborn();
			}
			if (blueorder[bluepoint % 5]== 1)
			{
				chengshi[numcity + 1].lan = new ninja(strength[1], power[1], bluepoint + 1, numcity + 1, 1);
				chengshi[numcity + 1].islan = 1; chengshi[numcity + 1].lan->showborn();
			}
			if (blueorder[bluepoint % 5] == 2)
			{
				chengshi[numcity + 1].lan = new iceman(strength[2], power[2], bluepoint + 1, numcity + 1, 1);
				chengshi[numcity + 1].islan = 1; chengshi[numcity + 1].lan->showborn();
			}
			if (blueorder[bluepoint % 5]== 3)
			{
				chengshi[numcity + 1].lan = new lion(strength[3], power[3], bluepoint + 1, numcity + 1, 1, element - strength[3]);
				chengshi[numcity + 1].islan = 1; chengshi[numcity + 1].lan->showborn();
			}
			if (blueorder[bluepoint % 5] == 4)
			{
				chengshi[numcity + 1].lan = new wolf(strength[4], power[4], bluepoint + 1, numcity + 1, 1);
				chengshi[numcity + 1].islan = 1; chengshi[numcity + 1].lan->showborn();
			}
			element -= strength[blueorder[bluepoint % 5]];
			bluepoint++;
		}
	}
};
R red;
B blue;

//每个小时第5分，lion逃跑
void city_run()
{
	for (int i = 0; i <= numcity + 1; i++)
	{
		if (chengshi[i].ishong)
		{
			if (chengshi[i].hong->name == "lion" and chengshi[i].hong->isable_run())
			{
				showtime();
				chengshi[i].hong->showrun();
				chengshi[i].ishong = 0;
			}
		}
		if (chengshi[i].islan)
		{
			if (chengshi[i].lan->name == "lion" and chengshi[i].lan->isable_run())
			{
				showtime();
				chengshi[i].lan->showrun();
				chengshi[i].islan = 0;
			}
		}
	}
}

//在每个小时的第10分：所有的武士朝敌人司令部方向前进一步。
void city_march()
{
	bool redmark = 0, bluemark = 0;
	if (chengshi[numcity].ishong)
	{
		blue.enemy_num += 1;
		chengshi[numcity].ishong = 0;
		chengshi[numcity+1].ishong = 1;
		redmark = 1;
		chengshi[numcity].hong->common_march();
		chengshi[numcity + 1].hong = chengshi[numcity].hong;
	}
	for (int i = numcity; i >= 1; i--)
	{
		if (chengshi[i - 1].ishong)
		{
			chengshi[i - 1].hong->common_march();
			chengshi[i].hong = chengshi[i - 1].hong;
			chengshi[i].ishong = 1; chengshi[i - 1].ishong = 0;
		}
	}
	if (chengshi[1].islan)
	{
		red.enemy_num += 1;
		chengshi[1].islan = 0;
		chengshi[0].islan = 1;
		bluemark = 1;
		chengshi[1].lan->common_march();
		chengshi[0].lan = chengshi[1].lan;
	}
	for (int i = 1; i <= numcity; i++)
	{
		if (chengshi[i + 1].islan)
		{
			chengshi[i + 1].lan->common_march();
			chengshi[i].lan = chengshi[i + 1].lan;
			chengshi[i].islan = 1; chengshi[i + 1].islan = 0;
		}
	}
	if (bluemark)
	{
		chengshi[0].lan->show_reach_head();
		if (red.enemy_num >= 2) { chengshi[0].lan->show_capture(); }
	}
	for (int i = 1; i <= numcity; i++)
	{
		if (chengshi[i].ishong) { chengshi[i].hong->showmarch(); }
		if (chengshi[i].islan) { chengshi[i].lan->showmarch(); }
	}
	if (redmark)
	{
		chengshi[numcity + 1].hong->show_reach_head();
		if (blue.enemy_num >= 2) { chengshi[numcity+1].hong->show_capture(); }
	}
}

//在每个小时的第20分：每个城市产出10个生命元。生命元留在城市，直到被武士取走。
void city_increase_element()
{
	for (int i = 1; i <= numcity; i++)
	{
		chengshi[i].left_element += 10;
	}
}

//在每个小时的第30分：如果某个城市中只有一个武士，那么该武士取走该城市中的所有生命元，并立即将这些生命元传送到其所属的司令部。
void city_get_element()
{
	for (int i = 1; i <= numcity; i++)
	{
		if (chengshi[i].ishong and !chengshi[i].islan)
		{
			red.element += chengshi[i].left_element;
			showtime();
			chengshi[i].hong->show_earn_element(chengshi[i].left_element);
			chengshi[i].left_element = 0;
		}
		if (!chengshi[i].ishong and chengshi[i].islan)
		{
			blue.element += chengshi[i].left_element;
			showtime();
			chengshi[i].lan->show_earn_element(chengshi[i].left_element);
			chengshi[i].left_element = 0;
		}
	}
}

void city_raise_flag(int color, int pos)
{
	chengshi[pos].flagcolor = color;
	cout << yanse[color] << " flag raised in city " << pos << endl;
}

//在每个小时的第35分，拥有arrow的武士放箭，对敌人造成伤害。两个相邻的武士可能同时放箭把对方射死。
void city_shot()
{
	for (int i = 1; i <= numcity; i++) { chengshi[i].isable = 0; }//初始均没有出现胜负,不考虑王城
	//展示射箭成果
	for (int i = 0; i <= numcity+1; i++)
	{
		if (i <= numcity)//提前写确保不会越界
		{
			if (chengshi[i].ishong and chengshi[i + 1].islan)
			{
				if (chengshi[i].hong->has_weapon[2])
				{
					if (chengshi[i + 1].lan->life <= chengshi[i].hong->arms[2]->firepower)
					{
						chengshi[i].hong->showshot_kill(chengshi[i + 1].lan);
					}
					else { chengshi[i].hong->showshot(); }
				}
			}
		}
		if (i >= 1)
		{
			if (chengshi[i].islan and chengshi[i - 1].ishong)
			{
				if (chengshi[i].lan->has_weapon[2])
				{
					if (chengshi[i-1].hong->life <= chengshi[i].lan->arms[2]->firepower)
					{
						chengshi[i].lan->showshot_kill(chengshi[i - 1].hong);
					}
					else { chengshi[i].lan->showshot(); }
				}
			}
		}
	}
	//真正扣血
	bool dead_blue[23] = { 0 };
	bool dead_red[23] = { 0 };//记录红蓝武士被箭射死的城市
	for (int i = 0; i <= numcity; i++)
	{
		if (chengshi[i].ishong and chengshi[i + 1].islan)
		{
			//红方有箭
			if (chengshi[i].hong->has_weapon[2])
			{
				//无论蓝色有没有箭，红色发射
				if (chengshi[i + 1].lan->life <= chengshi[i].hong->arms[2]->firepower)
				{
					chengshi[i + 1].dead = chengshi[i + 1].lan;
					chengshi[i + 1].lan->life = 0;
					chengshi[i + 1].islan = 0;
					dead_blue[i + 1] = 1;
				}
				else
				{
					chengshi[i + 1].lan->life -= chengshi[i].hong->arms[2]->firepower;
				}
				if (chengshi[i].hong->arms[2]->afterfight() == 3)
				{
					chengshi[i].hong->has_weapon[2] = 0;
				}
				//双方均有箭，蓝色发射
				if (chengshi[i+1].lan->has_weapon[2])
				{
					//cerr << '#' << endl;
					if (chengshi[i].hong->life <= chengshi[i+1].lan->arms[2]->firepower)
					{
						chengshi[i].dead = chengshi[i].hong;
						chengshi[i].hong->life = 0;
						chengshi[i].ishong = 0;
						dead_red[i] = 1;
					}
					else
					{
						chengshi[i].hong->life -= chengshi[i+1].lan->arms[2]->firepower;
					}
					if (chengshi[i+1].lan->arms[2]->afterfight() == 3)
					{
						chengshi[i+1].lan->has_weapon[2] = 0;
					}
				}
			}
			//红方无箭，蓝方有箭
			else if (chengshi[i + 1].lan->has_weapon[2])
			{
				if (chengshi[i].hong->life <= chengshi[i + 1].lan->arms[2]->firepower)
				{
					chengshi[i].dead = chengshi[i].hong;
					chengshi[i].hong->life = 0;
					chengshi[i].ishong = 0;
					dead_red[i] = 1;
				}
				else
				{
					chengshi[i].hong->life -= chengshi[i + 1].lan->arms[2]->firepower;
				}
				if (chengshi[i + 1].lan->arms[2]->afterfight() == 3)
				{
					chengshi[i + 1].lan->has_weapon[2] = 0;
				}
			}
		}
	}
	for (int i = 1; i <= numcity; i++)
	{
		if (chengshi[i].ishong and dead_blue[i])
		{
			chengshi[i].isable = 1;//出现胜负
			chengshi[i].redvictory += 1;
			chengshi[i].bluevictory = 0;
		}
		if (chengshi[i].islan and dead_red[i])
		{
			chengshi[i].isable = 1;//出现胜负
			chengshi[i].bluevictory += 1;
			chengshi[i].redvictory = 0;
		}
	}
}

//模拟战斗，1攻胜，-1守胜，0平局
int simulate_battle(warrior* gong, warrior* shou)
{
	int gong_mem_life = gong->life;
	int shou_mem_life = shou->life;
	gong->simulate_attack(shou);//只扣生命值
	if (shou->life <= 0)
	{
		shou->life = shou_mem_life;
		return 1;
	}
	shou->life = shou_mem_life;
	shou->simulate_fight_back(gong);//只扣生命值
	if (gong->life <= 0)
	{
		gong->life = gong_mem_life;
		return -1;
	}
	gong->life = gong_mem_life;
	return 0;
}
void using_bomb(warrior* gong, warrior* shou, int winner,int pos)
{
	if (winner == -1)
	{
		if (gong->has_weapon[1])
		{
			chengshi[pos].ishong = 0;
			chengshi[pos].islan = 0;
			gong->showbomb(shou);
		}
	}
	else if (winner == 1)
	{
		//cerr << yanse[gong->color] << gong->name << gong->code << '#' << gong->life << endl;
		if (shou->has_weapon[1])
		{
			chengshi[pos].ishong = 0;
			chengshi[pos].islan = 0;
			shou->showbomb(gong);
		}
	}
}
//每个小时第38分
void city_bomb()
{
	for (int i = 1; i <= numcity; i++)
	{
		if (chengshi[i].ishong and chengshi[i].islan)
		{
			int result = -2;
			if (chengshi[i].flagcolor == 0 or (chengshi[i].flagcolor == -1 and i % 2 == 1))
			{
				result = simulate_battle(chengshi[i].hong, chengshi[i].lan);
				using_bomb(chengshi[i].hong, chengshi[i].lan, result, i);
			}
			else
			{
				result = simulate_battle(chengshi[i].lan, chengshi[i].hong);
				using_bomb(chengshi[i].lan, chengshi[i].hong, result, i);
			}
		}
	}
}

void battle(warrior* gong, warrior* shou, int city_pos)
{
	gong->attack(shou);
	showtime(); gong->show_attack(shou);
	if (shou->life <= 0)
	{
		showtime(); shou->show_death();
		chengshi[city_pos].isable = 1;//出现胜负平
		chengshi[city_pos].dead = shou;//出现胜负
		if (shou->color == 0)
		{
			chengshi[city_pos].ishong = 0;
			chengshi[city_pos].bluevictory += 1;
			chengshi[city_pos].redvictory = 0;
		}
		else
		{
			chengshi[city_pos].islan = 0;
			chengshi[city_pos].redvictory += 1;
			chengshi[city_pos].bluevictory = 0;
		}
		for (int j = 0; j <= 2; j++)
		{
			if (shou->has_weapon[j])
			{
				chengshi[city_pos].left_weapon[j] = shou->arms[j];
			}
		}
	}
	else
	{
		shou->fight_back(gong);
		if(shou->name!="ninja")
		{
			showtime(); shou->show_fight_back(gong);
		}
		if (gong->life <= 0)
		{
			showtime(); gong->show_death();
			chengshi[city_pos].isable = 1;//出现胜负平
			chengshi[city_pos].dead = gong;//出现胜负
			if (gong->color == 0)
			{
				chengshi[city_pos].ishong = 0;
				chengshi[city_pos].bluevictory += 1;
				chengshi[city_pos].redvictory = 0;
			}
			else
			{
				chengshi[city_pos].islan = 0;
				chengshi[city_pos].redvictory += 1;
				chengshi[city_pos].bluevictory = 0;
			}
			for (int j = 0; j <= 2; j++)
			{
				if (gong->has_weapon[j])
				{
					chengshi[city_pos].left_weapon[j] = gong->arms[j];
				}
			}
		}
		else
		{
			//连胜中断
			chengshi[city_pos].bluevictory = 0;
			chengshi[city_pos].redvictory = 0;
			chengshi[city_pos].isable = 1;//出现胜负平
		}
	}
}
//在每个小时的第40分：在有两个武士的城市，会发生战斗。
void city_fight()//每小时第40分钟发生的事情
{
	for (int i = 1; i <= numcity; i++)
	{
		if ((chengshi[i].ishong and chengshi[i].islan)or chengshi[i].isable)//出现胜负平(箭干掉了人)
		{
			//为lion吸血设置记录器
			int mem_hong_life = 0;
			int mem_lan_life = 0;
			//分类讨论
			if(chengshi[i].ishong and chengshi[i].islan)//箭没有干掉人
			{
				mem_hong_life = chengshi[i].hong->life;
				mem_lan_life = chengshi[i].lan->life;
				if (chengshi[i].flagcolor == 0 or (chengshi[i].flagcolor == -1 and i % 2 == 1))
				{
					battle(chengshi[i].hong, chengshi[i].lan, i);
				}
				else
				{
					battle(chengshi[i].lan, chengshi[i].hong, i);
				}
			}
			else//箭干掉了人
			{
				if (chengshi[i].redvictory) { mem_lan_life = chengshi[i].dead->life; }
				if (chengshi[i].bluevictory) { mem_hong_life = chengshi[i].dead->life; }
			}
			//战斗胜利后应该发生的事情都会发生。
			if (chengshi[i].isable)//出现胜负平(发生过战斗)
			{
				//红方胜利
				if (chengshi[i].redvictory and chengshi[i].ishong)
				{
					chengshi[i].hong->special_win();//龙士气
					//缴获武器
					if (chengshi[i].hong->name == "wolf") { chengshi[i].hong->special_win(chengshi[i].dead); }
					if (chengshi[i].hong->name == "dragon")
					{
						if (chengshi[i].flagcolor == 0 or (chengshi[i].flagcolor == -1 and i % 2 == 1))
						{
							chengshi[i].hong->show_yeah();//欢呼
						}
					}
					//lion吸血
					if (chengshi[i].dead->name == "lion")
					{
						chengshi[i].hong->life += mem_lan_life;
					}
					//展示获取生命元
					showtime(); chengshi[i].hong->show_earn_element(chengshi[i].left_element);
					//升旗
					if (chengshi[i].redvictory >= 2 and chengshi[i].flagcolor != 0)
					{
						showtime(); city_raise_flag(0, i);
					}
				}
				//蓝方胜利
				else if (chengshi[i].bluevictory and chengshi[i].islan)
				{
					chengshi[i].lan->special_win();//龙士气
					if (chengshi[i].lan->name == "wolf") { chengshi[i].lan->special_win(chengshi[i].dead); }
					if (chengshi[i].lan->name == "dragon")
					{
						if (chengshi[i].flagcolor == 1 or (chengshi[i].flagcolor == -1 and i % 2 == 0))
						{
							chengshi[i].lan->show_yeah();//欢呼
						}
					}
					//展示获取生命元		
					showtime(); chengshi[i].lan->show_earn_element(chengshi[i].left_element);
					//lion吸血
					if (chengshi[i].dead->name == "lion")
					{
						chengshi[i].lan->life += mem_hong_life;
					}
					//升旗
					if (chengshi[i].bluevictory >= 2 and chengshi[i].flagcolor != 1)
					{
						showtime(); city_raise_flag(1, i);
					}
				}
				//平手
				else if (!chengshi[i].redvictory and !chengshi[i].bluevictory)
				{
					if (chengshi[i].hong->name == "dragon")
					{
						if (chengshi[i].flagcolor == 0 or (chengshi[i].flagcolor == -1 and i % 2 == 1))
						{
							chengshi[i].hong->show_yeah();//欢呼
						}
					}
					if (chengshi[i].lan->name == "dragon")
					{
						if (chengshi[i].flagcolor == 1 or (chengshi[i].flagcolor == -1 and i % 2 == 0))
						{
							chengshi[i].lan->show_yeah();//欢呼
						}
					}
					if (chengshi[i].ishong) { chengshi[i].hong->special_no_win(); }
					if (chengshi[i].islan) { chengshi[i].lan->special_no_win(); }
				}
			}
		}
		//如果武士在战斗中杀死敌人（不论是主动进攻杀死还是反击杀死）
		//则其司令部会立即向其发送8个生命元作为奖励，使其生命值增加8。
		//当然前提是司令部得有8个生命元。如果司令部的生命元不足以奖励所有的武士，则优先奖励距离敌方司令部近的武士。

		//如果某武士在某城市的战斗中杀死了敌人，则该武士的司令部立即取得该城市中所有的生命元。
		//注意，司令部总是先完成全部奖励工作，然后才开始从各个打了胜仗的城市回收生命元。
		//对于因司令部生命元不足而领不到奖励的武士，司令部也不会在取得战利品生命元后为其补发奖励。
	}
	//颁奖，不需要输出
	for (int i = 1; i <= numcity; i++)
	{
		if(chengshi[i].isable)
		{
			if (chengshi[i].bluevictory and chengshi[i].islan)
			{
				if (blue.element >= 8)
				{
					chengshi[i].lan->life += 8;
					blue.element -= 8;
				}
			}
		}
	}
	for (int i = numcity; i >= 1; i--)
	{
		if(chengshi[i].isable)
		{
			if (chengshi[i].redvictory and chengshi[i].ishong)
			{
				if (red.element >= 8)
				{
					chengshi[i].hong->life += 8;
					red.element -= 8;
				}
			}
		}
	}
	//真实获取生命元，不需要输出
	for(int i=1;i<=numcity;i++)
	{
		if (chengshi[i].redvictory and chengshi[i].ishong)
		{
			red.element += chengshi[i].left_element;
			chengshi[i].left_element = 0;
		}
		else if (chengshi[i].bluevictory and chengshi[i].islan)
		{
			blue.element += chengshi[i].left_element;
			chengshi[i].left_element = 0;
		}
	}
}

//在每个小时的第50分，司令部报告它拥有的生命元数量。
void headquater_report()
{
	showtime(); cout << red.element << " elements in red headquarter" << endl;
	showtime(); cout << blue.element << " elements in blue headquarter" << endl;
}

//在每个小时的第55分，每个武士报告其拥有的武器情况。
void weapon_report()
{
	for (int i = 0; i <= numcity+1; i++)
	{
		if (chengshi[i].ishong) { chengshi[i].hong->show_weapon(); }
	}
	for (int i = 0; i <= numcity+1; i++)
	{
		if (chengshi[i].islan) {  chengshi[i].lan->show_weapon(); }
	}
}
//新一组数据需要清零
void clean()
{
	red.enemy_num = 0; red.element = 0; red.redpoint = 0;
	blue.enemy_num = 0; blue.element = 0; blue.bluepoint = 0;
	for (int i = 0; i <= 22; i++)
	{
		chengshi[i].ishong = 0; chengshi[i].islan = 0;
		chengshi[i].left_element = 0;
		chengshi[i].flagcolor = -1;
		chengshi[i].isable = 0;
		chengshi[i].redvictory = 0;
		chengshi[i].bluevictory = 0;
	}
}

//const string mingzi[5] = { "dragon","ninja","iceman","lion","wolf" };


const int key[10] = { 0,5,10,20,30,35,38,40,50,55 };
int main()
{
	int group;
	cin >> group;
	for (int i = 1; i <= group; i++)
	{
		cout << "Case " << i << ':' << endl;
		cin>> initial_elements>>numcity>>arrow_power>>loyalty_drop>>time_limit;
		for (int j = 0; j <= 4; j++)
		{
			cin >> strength[j];
		}
		for (int j = 0; j <= 4; j++)
		{
			cin >> power[j];
		}
		clean();
		red.element = initial_elements;
		blue.element = initial_elements;
		now = 0;
		for (int k = 0; now <= time_limit and red.enemy_num < 2 and blue.enemy_num < 2; k++)
		{
			for (int j = 0; j <= 9 and red.enemy_num < 2 and blue.enemy_num < 2; j++)
			{
				now = 60 * k + key[j];
				if (now > time_limit) { break; }
				if (j == 0)
				{
					red.make_warrior();
					blue.make_warrior();
				}
				if (j == 1) { city_run(); }
				if (j == 2) { city_march(); }
				if (j == 3) { city_increase_element(); }
				if (j == 4) { city_get_element(); }
				if (j == 5) { city_shot(); }
				if (j == 6) { city_bomb(); }
				if (j == 7) { city_fight(); }
				if (j == 8) { headquater_report(); }
				if (j == 9) { weapon_report(); }
			}
		}
	}
}
//有待实现:2,5,9,10,11,14,15