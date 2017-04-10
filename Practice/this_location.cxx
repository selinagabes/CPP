#include <iostream>
#include <typeinfo>
#include <boost/type_index.hpp>
using namespace std;

class Q7{
  public:
    Q7(){ cout <<"Q7() constructor \t "<< boost::typeindex::type_id_runtime(this).pretty_name() << " this" << endl;}
    void b() const { cout <<"b() const \t " << boost::typeindex::type_id_runtime(this).pretty_name()<<" this" << endl;}
    void c() { cout <<"c() \t "<< boost::typeindex::type_id_runtime(this).pretty_name()<<" this" << endl;}
    void d() & { cout <<"d() & \t "<< boost::typeindex::type_id_runtime(this).pretty_name()<<" this" << endl;}
    void e() const& { cout <<"e() const& \t "<< boost::typeindex::type_id_runtime(this).pretty_name()<<" this" << endl;}

  void f() && { cout <<"f() && \t "<< boost::typeindex::type_id_runtime(this).pretty_name()<<" this" << endl;}
//static void g() { cout <<"static g() \t "<< typeid(this).name() << endl;}
};


int main(){
  Q7 q;
  q.b();
  q.c();
  cout <<"q.c() \t " << boost::typeindex::type_id_runtime(q).pretty_name() << endl;
  q.d();
  q.e();
  Q7().f();
//Q7&& test = q.f();
//q.g();
  return 0;
}
