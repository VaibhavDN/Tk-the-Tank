#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <math.h>
#include <string>
using namespace std;
using namespace sf;

/******Attack******/
class attack{
    public:
    sf::CircleShape heat;
    attack(int x,int y)
    {
        /**High explosive anti-tank (HEAT)**/
        heat.setRadius(5);
        heat.setFillColor(sf::Color::Magenta);
        heat.setOrigin(5,5);
        heat.setPosition(x,y);
    }
    void setNewPosition(float x,float y);
};

void attack::setNewPosition(float x,float y)
{
    heat.setPosition(x,y);
}

/******Ground******/
class ground{
    public:
    sf::RectangleShape rect1;
    sf::RectangleShape rect2;
    sf::RectangleShape background;
    sf::Texture texture;
    ground()
    {
        texture.loadFromFile("bg.jpg");
        rect1.setSize(sf::Vector2f(683,384));
        rect2.setSize(sf::Vector2f(683,384));
        rect1.setPosition(sf::Vector2f(0,384));
        rect2.setPosition(sf::Vector2f(683,384));
        rect1.setFillColor(sf::Color::Magenta);
        rect2.setFillColor(sf::Color::Magenta);
        background.setSize(sf::Vector2f(1366,768));
        background.setTexture(&texture);
    }
};

/******Tanks******/
class tanks{
    public:
    sf::RectangleShape tank;
    sf::Texture tnk;
    sf::Sprite sprite;
    tanks(int x,int y)
    {
        if(!tnk.loadFromFile("tk.png"))
        {
            cout<<"tk ggfied"<<endl;
        }
        sprite.setTexture(tnk);
        sprite.setPosition(x,y);
        tank.setPosition(x,y);
        tank.setSize(sf::Vector2f(80,60));
        //tank.setFillColor(sf::Color::Red);
    }
};

/******Health******/
class health{
    public:
    sf::RectangleShape tankHealth;
    health(int x,int y,float damage)
    {
        tankHealth.setSize(sf::Vector2f(40-damage,10));
        tankHealth.setPosition(x,y-20);
        tankHealth.setFillColor(sf::Color::Yellow);
    }
};

class Rank{
    public:
    sf::Text text,text2;
    sf::Font font;
    sf::String str;
    sf::String str2;
    string s;
    int i,x=20,y=40;
    vector<sf::Text>textVec;
    Rank(int noofplayers)
    {
        font.loadFromFile("FiraSans-Book.otf");
        text.setFont(font);
        str2="Ranking:";
        text2.setFont(font);
        text2.setString(str2);
        text2.setPosition(x,2);
        text2.setFillColor(sf::Color::Blue);
        for(i=0;i<noofplayers;i++)
        {
            s=to_string(i+1);
            str="Player "+s;
            cout<<"run"<<i<<endl;
            text.setString(str);
            text.setPosition(x,y);
            text.setFillColor(sf::Color::Red);
            text.setCharacterSize(20);
            textVec.push_back(text);
            y+=30;
        }
    }
    void changeRank(int index[],int noofplayers)
    {
        y=40;
        for(i=0;i<noofplayers;i++)
        {
            textVec[index[noofplayers-1-i]].setPosition(x,y);
            y+=30;
        }
        
    }
};

/******Projectile******/
vector<float> projectile(sf::RectangleShape tank1,sf::RectangleShape tank2)
{
    vector<float>result;
    float range,x,reduction,u,y,theta=45,totalTime;
    range=abs(tank2.getPosition().x-tank1.getPosition().x);
    totalTime=range/2;
    x=3;    //x=u*cos(theta)
    u=x/cos(theta*(3.14/180));  //u=y here
    y=u*sin(theta*(3.14/180));
    reduction=(y/(totalTime)*x);
    cout<<"range: "<<range<<" reduction: "<<reduction<<" x: "<<x<<" y: "<<y<<endl;
    result.push_back(y);
    result.push_back(x);
    result.push_back(reduction);
    return result;
}

/******Collision******/
bool checkCollision(attack a1,sf::RectangleShape tank)
{
    if(a1.heat.getPosition().x>tank.getPosition().x-1 && a1.heat.getPosition().y>tank.getPosition().y && a1.heat.getPosition().x<(tank.getPosition().x+tank.getSize().x))
    {
        return true;
    }
    else
    {
        return false;
    }
}

/******Find minimun distance******/
int findMinDistance(vector<tuple<tanks,health> >player,int currentPlayer,int noofplayers)
{
    int nearest=10000,i=0,index=0;
    for(i=0;i<noofplayers;i++)
    {
        if((abs((get<0>(player[currentPlayer]).tank.getPosition().x)-(get<0>(player[i]).tank.getPosition().x))<nearest) && i!=currentPlayer && (get<1>(player[i]).tankHealth.getSize().x)>0)
        {
            nearest=abs((get<0>(player[currentPlayer]).tank.getPosition().x)-(get<0>(player[i]).tank.getPosition().x));
            index=i;
        }
    }
    cout<<"index: "<<index<<" nearest: "<<nearest<<endl;
    return index;
}

/******Find minimum health*******/
int findMaxHealth(vector<tuple<tanks,health> >player,int currentPlayer,int noofplayers)
{
    int i,index,repeat=0;
    
    float maxHealth=0;
    for(i=0;i<noofplayers;i++)
    {
        if((get<1>(player[i]).tankHealth.getSize().x)>maxHealth && (get<1>(player[i]).tankHealth.getSize().x)>0 && i!=currentPlayer)
        {
            maxHealth=(get<1>(player[i]).tankHealth.getSize().x);
            index=i;
            repeat=0;
        }
        else if((get<1>(player[i]).tankHealth.getSize().x)==maxHealth && i!=currentPlayer)
        {
            repeat=1;
        }
    }
    if(repeat==0)
    {
        return index;
    }
    else
    {
        return 911;
    }
}

void heapify(float *healthArray,int n,int i,int *index)
{
    int largest=i;
	int l=2*i+1;
	int r=2*i+2;

	if (l<n && healthArray[l] > healthArray[largest])
    {
        largest=l;
    }

	if (r < n && healthArray[r] > healthArray[largest])
    {
        largest=r;
    }

	if (largest!=i) 
	{ 
		swap(healthArray[i],healthArray[largest]);
        swap(index[i],index[largest]);
		heapify(healthArray, n, largest,index);
	} 
}

void heapSort(float *healthArray,int n,int *index)
{
    for(int i=n/2-1;i>=0;i--)
    {
        heapify(healthArray,n,i,index);
    }
    for (int i=n-1; i>=0; i--) 
	{ 
		swap(healthArray[0], healthArray[i]);
        swap(index[0], index[i]);
		heapify(healthArray, i, 0,index);
	} 
}

/******Main******/
int main()
{
    /*********Variables**********/
    bool shoot=false,collision=false;
    int noofplayers=0,i,temp,playersturn=0,nearestIndex=0,leastHealth=0,index[10];
    float x,reduction,y,direction=1,projectileLimit,healthArray[10];
    vector<tuple<tanks,health> >player;
    vector<float>result;
    srand(time(0));
    sf::Texture tnk;
    tnk.loadFromFile("tk.png");
    /****************************/
    cout<<"Please enter no of players (5 recommended): ";
    cin>>noofplayers;
    while(noofplayers>5 || noofplayers<2)
    {
        cout<<"These many tanks wouldn't fit in the screen, please enter a number between 1-5 (both inclusive): ";
        cin>>noofplayers;
    }
    sf::RenderWindow window(sf::VideoMode(1366,768),"Tanks");
    window.setKeyRepeatEnabled(false);
    window.setFramerateLimit(60);
    sf::Event event;
    /***********Objects**********/
    ground g1;
    temp=0;
    for(i=0;i<noofplayers;i++)
    {
        temp+=260-(rand()%100);
        tanks t(temp,550);//364
        health h(t.tank.getPosition().x,t.tank.getPosition().y,0);
        player.push_back(make_tuple(t,h));
    }
    Rank rank(noofplayers);
    attack a1((get<0>(player[playersturn])).tank.getPosition().x+10,(get<0>(player[playersturn]).tank).getPosition().y);

/************Game Loop**************/
    while(window.isOpen())
    {
        while(window.pollEvent(event))
        {
            switch(event.type)
            {
                case sf::Event::Closed:
                window.close();
                break;
            }
        }
        fflush(stdin);
        if(playersturn==0 && shoot==false)
        {
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            {
                x+=0.1; //Power of the shot increased
                cout<<x<<endl;
            }
            else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            {
                x-=0.1; //Power of the shot decreased
            }
            else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            {
                direction=1; //Shoot towards right
            }
            else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            {
                direction=-1; //Shoot towards left
            }

            if((get<1>(player[playersturn]).tankHealth.getSize().x<=0))
            {
                playersturn++;
                continue;
            }

            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
            {
                shoot=true;
                y=x;
                playersturn++;
                reduction=0.0810165;
                a1.setNewPosition((get<0>(player[playersturn])).tank.getPosition().x,get<0>(player[playersturn]).tank.getPosition().y);
            }
        }
        else if(playersturn!=0 && shoot==false)
        {
            shoot=true;
            if(playersturn<noofplayers-1)
            {
                playersturn++;
                if((get<1>(player[playersturn]).tankHealth.getSize().x<=0))
                {
                    continue;
                }
            }
            else
            {
                playersturn=0;
            }
            
            leastHealth=findMaxHealth(player,playersturn,noofplayers);
            if(leastHealth==911)
            {
                nearestIndex=findMinDistance(player,playersturn,noofplayers);
            }
            else
            {
                nearestIndex=leastHealth;
            }

            if(nearestIndex<playersturn)
            {
                direction=-1;
            }
            else
            {
                direction=+1;
            }
            result=projectile((get<0>(player[playersturn])).tank,(get<0>(player[nearestIndex]).tank));
            y=result[0];
            x=result[1];
            x-=rand()%2;
            reduction=result[2];
            projectileLimit=-y;
            a1.setNewPosition((get<0>(player[playersturn])).tank.getPosition().x,get<0>(player[playersturn]).tank.getPosition().y);
            cout<<"Shot"<<endl;
        }
        
        if(shoot==true && projectileLimit-0.2<y)
        {
            window.draw(a1.heat);
            a1.heat.move(direction*x,-(y-reduction));
            y-=reduction;
            collision=checkCollision(a1,(get<0>(player[nearestIndex])).tank);
            if(collision==true && (get<1>(player[nearestIndex]).tankHealth.getSize().x)>0)
            {
                get<1>(player[nearestIndex]).tankHealth.setSize(sf::Vector2f(get<1>(player[nearestIndex]).tankHealth.getSize().x-8,get<1>(player[nearestIndex]).tankHealth.getSize().y));
                cout<<"After Collision with: "<<nearestIndex<<" health remaining is: "<<get<1>(player[nearestIndex]).tankHealth.getSize().x<<endl;
                shoot=false;
            }
            //cout<<"x: "<<x<<" y: "<<y<<" projectileLimit"<<projectileLimit<<" reduction: "<<reduction<<" collision: "<<collision<<endl;
        }
        else
        {
            shoot=false;
        }

//************Ranking*********************
        for(i=0;i<noofplayers;i++)
        {
            index[i]=i;
            healthArray[i]=get<1>(player[i]).tankHealth.getSize().x;
            //cout<<healthArray[i];
        }

        heapSort(&healthArray[0],noofplayers,&index[0]);
        
        for(i=0;i<noofplayers;i++)
        {
            cout<<" "<<healthArray[i]<<" ";
        }
        cout<<"\n";
        for(i=0;i<noofplayers;i++)
        {
            cout<<":"<<index[i]<<":";
        }
        rank.changeRank(index,noofplayers);

//************Drawing***************
        window.display();
        window.clear(sf::Color::White);
        window.draw(g1.background);
        //window.draw(g1.rect1);
        //window.draw(g1.rect2);
        for(i=0;i<noofplayers;i++)
        {
            (get<0>(player[i])).tank.setTexture(&tnk);
            window.draw((get<0>(player[i])).tank);
            window.draw((get<1>(player[i]).tankHealth));
        }
        for(i=0;i<noofplayers;i++)
        {
            window.draw(rank.textVec[i]);
        }
        window.draw(rank.text2);
    }
    return 0;
}