#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>

using namespace eosio;

CONTRACT onnotify: public contract {
    public:
    using contract::contract;

    ACTION dummy(){}
    
    [[eosio::on_notify("eosio.token::transfer")]]
    void ontransfer(name from, name to, asset quantity, std::string memo) {
            check(from == get_self(),"no out");//from 과 내 자신 비교

            outs myTable(get_self(), get_self().value);
            if(myTable.begin() == myTable.end()) {//테이블이 존재하지 않는경우
                myTable.emplace(from, [&](auto& row) {//quantity를 balance에 추가
                    row.balance = quantity;
                });
            } else {//테이블이 있는 경우
                auto itr = myTable.begin();//수정하기 전에 테이블을 찾는다(이 경우, 원소가 하나일 것이므로 begin 함수를 사용해 주어야 한다.)
                //그리고 나서 테이블 수정
                myTable.modify(itr, from, [&](auto& row) {
                    row.balance += quantity;
                });
            }

    }
    private:
    TABLE outstruct { //내가 사용할 테이블
        asset balance;

        uint64_t primary_key() const { return balance.symbol.code().raw(); }
    };

    typedef multi_index<"out"_n, outstruct> outs;
};