#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>

using namespace eosio;

CONTRACT onnotify: public contract {
public:
using contract::contract;

ACTION dummy() {}

[[eosio::on_notify("eosio.token::transfer")]]
void ontransfer(name from, name to, asset quantity, std::string memo) {
if(from == get_self()) {
outs myTable(get_self(), get_self().value);
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
} else {
print("you are not from");
}
}
private:
TABLE outstruct {
asset balance;

uint64_t primary_key() const { return balance.symbol.code().raw(); }
};

typedef multi_index<"out"_n, outstruct> outs;
};