
//=======================================================================
// Copyright 2001 Jeremy G. Siek, Andrew Lumsdaine, Lie-Quan Lee,
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//======================================================================
#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include "skiplist.h"
using namespace std;

template<unsigned int lv = 1>
struct carrier{
    unsigned int state[lv];
    unsigned int freq;
    int prev_idx;
    carrier():freq(0), prev_idx(-1){
        for(unsigned int i = 0; i < lv; ++i){
            state[i] = 0;
        }
    }
    ~carrier(){
    }
    carrier add(unsigned int curlv, int f){
        if(curlv > lv || curlv == 0){
            cout<<"level exceed max"<<endl;
            return carrier();
        }
        carrier res;
        
        for(unsigned int i = 0; i < curlv - 1; ++i){
            res.state[i] = 0;
        }
        res.state[curlv-1] = this->state[curlv-1]+1;
        for(unsigned int i = curlv; i < lv; ++i){
            res.state[i] = this->state[i];
        }
        
        res.freq = this->freq;
        for(unsigned int i = 0; i < lv; ++i){
            res.freq += f*res.state[i];
        }
        return res;
    };
    
    void print() const{
        copy(state, state+lv, ostream_iterator<unsigned int>(cout, " "));
        cout<<" : "<<prev_idx<<" : "<<freq;
    };
    
    bool equal_state(const carrier<lv>& val) const{
        for(unsigned int i = 0; i< lv; ++i)
            if(state[i] != val.state[i])
                return false;
        return true;
    };
    
    bool equal(const carrier<lv>& val) const {
        return freq == val.freq && equal_state(val);
    }
    
    bool operator < (const carrier<lv>& val) const {
        for(int i = lv-1; i >= 0; --i){
            if(state[i] < val.state[i])
                return true;
            else if(state[i] > val.state[i])
                return false;
        }
        return false;
    }
    
    bool operator == (const carrier<lv>& val) const{
        return equal_state(val);
    }
    
    bool operator > (const carrier<lv>& val) const{
        return !(this->operator<(val) || this->operator==(val));
    }
    
};

const int maxlv = 4;
typedef carrier<maxlv> carry;

vector<carry> make_Zipf(const map<int, int>& weight){
    auto map_iter = weight.begin();
    
    carry first_carry;
    int n = int(weight.size());
    int b =0;
    first_carry.freq = (map_iter++)->second;
    
    vector<skiplist<carry>> vv;
    skiplist<carry>::average_skip_policy a = skiplist<carry>::average_skip_policy();
    skiplist<carry> sl(&a);
    sl.insert(first_carry);
    vv.push_back(sl);
    
    carry prev_carry;
    carry tmp[maxlv];
    int min = 1000000;
    
    for(; map_iter != weight.end(); ++map_iter){
        
        int prev_idx = 0;
        skiplist<carry>::average_skip_policy a = skiplist<carry>::average_skip_policy();
        skiplist<carry> sl(&a);
        for(auto iter = (vv.back()).begin(); iter != (vv.back()).end(); ++iter,++prev_idx){
            prev_carry = *iter;
            
            for(unsigned int lv = 0; lv < maxlv ; ++lv){
                tmp[lv] = prev_carry.add(lv+1, map_iter->second);
                tmp[lv].prev_idx = prev_idx;
                
                if(tmp[lv].state[lv] > n/(lv+1) - (maxlv - lv-1)){
                    continue;
                }else{
                    sl.insert(tmp[lv]);
                }
                
            }
        }
        if(sl.begin() != sl.end()){
            skiplist<carry>::iterator __first = sl.begin();
            skiplist<carry>::iterator __last = sl.end();
            skiplist<carry>::iterator __next = __first;
            while (++__next != __last) {
                if (__first->equal_state(*__next)){
                    if(__first->freq > __next->freq){
                        __first->freq = __next->freq;
                        __first->prev_idx = __next->prev_idx;
                    }
                    sl.erase(__next);
                }
                else
                    __first = __next;
                __next = __first;
            }
        }
        
        
        vv.push_back(sl);
        cout<<b++<<" end of states : "<<sl.size()<<" : "<<sl.size() - vv[vv.size()-2].size()<<endl;
    }
    
    cout<<endl<<"start read state"<<endl;
    vector<carry> res;
    min = 1000000;
    int state_idx = 0;
    int i = 0;
    for(auto iter = vv.back().begin(); iter != vv.back().end(); ++i, ++iter){
        if(iter->freq < min){
            min = iter->freq;
            state_idx = i;
        }
    }
    int j = int(vv.size()-1);
    skiplist<carry>::iterator cur;
    while(state_idx != -1 && j >= 0){
        cur = vv[j].begin();
        while(state_idx-- != 0){
            ++cur;
        }
        res.push_back(*cur);
        state_idx = cur->prev_idx;
        --j;
    }
    reverse(res.begin(), res.end());
    return res;
}

int main()
{
    map<int, int> weight;
    for(int i = 0; i< 50; ++i){
        weight.insert(make_pair(i, 1));
    }
    
    weight[26] = 1000;
    weight[28] = 1000;
    weight[27] = 1000;
    vector<carry> result = make_Zipf(weight);
    int i = -1;
    for(auto iter = result.begin(); iter != result.end(); ++iter, ++i){
        cout<< i << " : ";
        copy(iter->state, iter->state+maxlv, ostream_iterator<unsigned int>(cout, " "));
        cout<<" : "<<iter->freq;
        cout<<endl;
    }
    return 0;
}



y = (x-1)*Exp(x)+2a*(x-1);

- y*Exp(y)
x-1 > lnx
1 > (lnx)/(x-1)  for all x > 1

