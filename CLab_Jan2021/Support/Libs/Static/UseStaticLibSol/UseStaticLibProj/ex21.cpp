#include <iostream>
#include "./ex21string.hpp"
using namespace std;
using namespace ism;

void main()
{
	//ism::string s1("First string ");
	//ism::string s2("and the second one.");
	////while(1) {
	//	s2 = s1;
	////}
	std::string s11("First string again ");
	std::string s12("and the second one.");
	s11 += s12;
	//cout<<s1.getCString()<<endl;
	//cout<<s1<<endl;
	//cout<<s2<<endl;
	cout<<s11<<endl;
	cout<<s12<<endl;


	ism::string s1("The third string ");
	ism::string s2("and the next one.");

	s1 += s2;

	cout<<"s1="<<s1;

	ism::string s3, s4, s5;
	cout<<endl;
	cout<<"s3="; 
	cin>>s3;
	cout<<"s4="; 
	cin>>s4;

	s5=s3+s4;

	cout<<"\n s5="<<s5;

	s5 = s5 + " new text added to the string";		//commutativity
	s5 = " the final text " + s5;	//commutativity

	cout<<"\n s5="<<s5;
}