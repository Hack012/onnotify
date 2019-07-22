#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>

using namespace eosio;

CONTRACT onnotify: public contract {
public:
using contract::contract;

ACTION dummy() {}

[[eosio::on_notify("eosio.token::transfer")]] //to: 보낼 사람, from: 보내는사람
void ontransfer(name from, name to, asset quantity, std::string memo) {
if(from == get_self()) {
outs myTable(get_self(), get_self().value); //출금액 확인하는 테이블
if(myTable.begin() == myTable.end()) {
myTable.emplace(from, [&](auto& row) {
row.balance = quantity;
});
} else {
auto itr = myTable.begin();
myTable.modify(itr, from, [&](auto& row) {
row.balance += quantity;
});
}
} else if(to == get_self()) { //입금액 확인하는 테이블 
ins myTable(get_self(), get_self().value);
if(myTable.begin() == myTable.end()) {
    myTable.emplace(to, [&](auto& row) {
        row.balance = quantity;
    });
} else {
    auto itr = myTable.begin();
    myTable.modify(itr, to, [&](auto& row) {
        row.balance += quantity;
    });
}
}
}
private:
TABLE outstruct {
asset balance;

uint64_t primary_key() const { return balance.symbol.code().raw(); }
};

typedef multi_index<"out"_n, outstruct> outs;
typedef multi_index<"in"_n, outstruct> ins; //테이블 하나를 만들어줬다.
};