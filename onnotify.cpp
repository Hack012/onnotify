#include<eosio/eosio.hpp>
#include<eosio/asset.hpp>

using namespace eosio;

CONTRACT onnotify: public contract {
public:
using contract::contract;

ACTION dummy() {}

[[eosio::on_notify("eosio.token::transfer")]]
void ontransfer(name from, name to, asset quantity, std::string memo) {
if(from == get_self()) {
Receiver receiver(get_self(), get_self().value);
auto itr = receiver.find(to.value);
if(itr == receiver.end()) {
receiver.emplace(from, [&](auto& row) {
row.user = to;
row.balance = quantity;
});
} else {
receiver.modify(itr, from, [&](auto& row) {
row.balance += quantity;
});
}
} else {
Sender sender(get_self(), get_self().value);
auto itr = sender.find(from.value);
if(itr == sender.end()) {
sender.emplace(to, [&](auto& row) {
row.user = from;
row.balance = quantity;
});
} else {
sender.modify(itr, to, [&](auto& row) {
row.balance += quantity;
});
}
}
}
private:
TABLE trxlist {
name user;
asset balance;

uint64_t primary_key() const { return user.value; }
};

typedef multi_index<"eossender"_n, trxlist> Sender; //나한테 보냈다
typedef multi_index<"eosreceiver"_n, trxlist> Receiver;//내가보낸 사람이 받았다!
};