//
//  skiplist.h
//  datastruct
//
//  Created by haodong bai on 7/22/16.
//  Copyright © 2016 haodong bai. All rights reserved.
//


/*
 need to improve:
 merge use an array to record flow ? transfer
 sort use list as merge unit instead of skiplist
 sort and intersect return a pointer
 make_Zipf and Zipf_skip_policy
 
 two container wrapper
 increment container
 variable byte
 */

#ifndef skiplist_h
#define skiplist_h
#pragma once

#include <vector>
#include <random>
#include <list>
using namespace std;

#define MaxLevel 8
struct _skiplist_node_base;
typedef _skiplist_node_base* _p_skiplist_node_base;

struct _skiplist_node_base {
    _p_skiplist_node_base* _M_next;
    _p_skiplist_node_base* _M_prev;
    unsigned int _lv = 0;
};

template <class _Tp>
struct _skiplist_node : public _skiplist_node_base {
    _Tp _M_data;
};

struct _skiplist_iterator_base {
    typedef size_t                     size_type;
    typedef ptrdiff_t                  difference_type;
    typedef bidirectional_iterator_tag iterator_category;
    
    _p_skiplist_node_base _M_node;
    
    _skiplist_iterator_base(_p_skiplist_node_base __x):_M_node(__x){
    }
    _skiplist_iterator_base() {}
    
    void _M_incr() { _M_node = _M_node->_M_next[0]; }
    void _M_decr() { _M_node = _M_node->_M_prev[0]; }
    
    bool operator==(const _skiplist_iterator_base& __x) const {
        return _M_node == __x._M_node;
    }
    bool operator!=(const _skiplist_iterator_base& __x) const {
        return _M_node != __x._M_node;
    }
};

template<class _Tp, class _Ref, class _Ptr>
struct _skiplist_iterator : public _skiplist_iterator_base {
    typedef _skiplist_iterator<_Tp,_Tp&,_Tp*>             iterator;
    typedef _skiplist_iterator<_Tp,const _Tp&,const _Tp*> const_iterator;
    typedef _skiplist_iterator<_Tp,_Ref,_Ptr>             _Self;
    
    typedef _Tp value_type;
    typedef _Ptr pointer;
    typedef _Ref reference;
    typedef _skiplist_node<_Tp> _Node;
    
    _skiplist_iterator(_Node* __x) : _skiplist_iterator_base(__x) {}
    _skiplist_iterator() {}
    _skiplist_iterator(const iterator& __x) : _skiplist_iterator_base(__x._M_node) {}
    
    reference operator*() const { return ((_Node*) _M_node)->_M_data; }
    
    pointer operator->() const { return &(operator*()); }
    
    _Self& operator++() {
        this->_M_incr();
        return *this;
    }
    _Self operator++(int) {
        _Self __tmp = *this;
        this->_M_incr();
        return __tmp;
    }
    _Self& operator--() {
        this->_M_decr();
        return *this;
    }
    _Self operator--(int) {
        _Self __tmp = *this;
        this->_M_decr();
        return __tmp;
    }
};

inline bidirectional_iterator_tag
iterator_category(const _skiplist_iterator_base&)
{
    return bidirectional_iterator_tag();
}

template <class _Tp, class _Ref, class _Ptr>
inline _Tp*
value_type(const _skiplist_iterator<_Tp, _Ref, _Ptr>&)
{
    return 0;
}

inline ptrdiff_t*
distance_type(const _skiplist_iterator_base&)
{
    return 0;
}


template <class _Tp>
class _skiplist_base
{
public:
    
    _skiplist_base():_skiplist_base(MaxLevel) { }
    
    _skiplist_base(unsigned int _lv):_maxlv(_lv) {
        _maxlv = _lv;
        _M_node = _M_get_node();
        if(_M_set_level(_M_node, _maxlv)){
            _M_node->_lv = _lv;
            for(unsigned int i = 0; i < _lv; ++i){
                _M_node->_M_next[i] = _M_node;
                _M_node->_M_prev[i] = _M_node;
            }
        }else{
            _M_put_node(_M_node);
        }
    }
    
    ~_skiplist_base() {
        clear();
        _M_put_node(_M_node);
    }
    
    void clear(){
        _skiplist_node<_Tp>* __cur = (_skiplist_node<_Tp>*) _M_node->_M_next[0];
        while (__cur != _M_node) {
            _skiplist_node<_Tp>* __tmp = __cur;
            __cur = (_skiplist_node<_Tp>*) __cur->_M_next[0];
            _M_put_node(__tmp);
        }
        _M_node->_M_next[0] = _M_node;
        _M_node->_M_prev[0] = _M_node;
    }
    
protected:
    _skiplist_node<_Tp>* _M_get_node() { return new _skiplist_node<_Tp>; }
    _skiplist_node<_Tp>* _M_get_node(const _Tp& __x) {
        _skiplist_node<_Tp>* tmp = new _skiplist_node<_Tp>;
        tmp->_M_data = __x;
        return tmp;
    }
    bool _M_set_level(_skiplist_node<_Tp>* _node, unsigned int _lv ) {
        if(_lv == 0) return false;
        if(_lv > this->_maxlv){
            cout<<"level excced max"<<endl;
        }
        
        _node->_M_next = new _p_skiplist_node_base[_lv];
        _node->_M_prev = new _p_skiplist_node_base[_lv];
        if(_node->_M_next == NULL || _node->_M_prev == NULL){
            cout<<"Allocate failed"<<endl;
        }
        //cout<<"allocate success: "<<_lv<<endl;
        _node->_lv = _lv;
        return true;
    }
    void _M_put_node(_skiplist_node<_Tp>* __p) {
        if(__p->_lv != 0){
            delete[] __p->_M_next;
            delete[] __p->_M_prev;
            __p->_lv = 0;
        }
        delete __p;
    }
    
protected:
    _skiplist_node<_Tp>* _M_node;
    unsigned int _maxlv = MaxLevel;
};


template <class _Tp>
class skiplist : protected _skiplist_base<_Tp> {
private:
    typedef _skiplist_base<_Tp> _Base;
protected:
    typedef void* _Void_pointer;
public:
    typedef _Tp value_type;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef _skiplist_node<_Tp> _Node;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef _skiplist_iterator<_Tp,_Tp&,_Tp*>             iterator;
    typedef _skiplist_iterator<_Tp,const _Tp&,const _Tp*> const_iterator;
    typedef reverse_iterator<const_iterator> const_reverse_iterator;
    typedef reverse_iterator<iterator>       reverse_iterator;
    
public:
    struct _skip_policy{
        virtual unsigned int get_level(skiplist<_Tp>* __list, skiplist<_Tp>::iterator& __pos) = 0;
    };
    
    struct random_skip_policy: public _skip_policy{
        double alpha;
        std::uniform_real_distribution<double> distribution;
        std::default_random_engine engine;
        
        random_skip_policy(double a = 0.2):alpha(a),distribution(0,1/(1-a)){
            
        }
        unsigned int get_level(skiplist<_Tp>* __list, skiplist<_Tp>::iterator& __pos){
            
            int _lv = log(1+distribution(engine)*(alpha-1))/log(alpha) + 1;
            return _lv>__list->_maxlv?__list->_maxlv:_lv;
        }
    };
    struct empty_skip_policy: public _skip_policy{
        unsigned int get_level(skiplist<_Tp>* __list, skiplist<_Tp>::iterator& __pos){
            return 1;
        }
    };
    struct average_skip_policy: public _skip_policy{
        struct carrier{
            unsigned int digit;
            const unsigned int scale;
            vector<unsigned int> num;
            carrier(unsigned int s=5, unsigned int d=MaxLevel):scale(s),digit(d){
                num.resize(d,0);
            }
            unsigned int add(){
                unsigned int i = 0;
                ++num[i];
                while(i < digit && num[i] == scale){
                    num[i] = 0;
                    ++num[++i];
                }
                ++i;
                return i>digit?digit:i;
            }
        };
        carrier count;
        average_skip_policy(unsigned int s = 5, unsigned int lv = MaxLevel):count(s,lv){}
        
        unsigned int get_level(skiplist<_Tp>* __list, skiplist<_Tp>::iterator& __pos){
            return count.add();
        }
        
    };
    
    typedef average_skip_policy default_skip_policy;
protected:
    _skip_policy* policy;
    
protected:
    using _Base::_M_node;
    using _Base::_M_put_node;
    using _Base::_M_get_node;
    using _Base::_M_set_level;
    
    
    
    _Node* _insert_level(_Node* __x, iterator& __pos){
        if(__x->_lv > this->_maxlv){
            cout<<"level exceed MAX"<<endl;
        }
        int _xlv = __x->_lv;
        vector<_Node*> left_next;
        _left_snapshot(iterator(_M_node), __pos, _xlv, left_next);
        
        for(unsigned int i = 0; i < _xlv && i < left_next.size(); ++i){
            _Node* tmp_next = (_Node*)left_next[i]->_M_next[i];
            tmp_next->_M_prev[i] = __x;
            __x->_M_next[i] = tmp_next;
            left_next[i]->_M_next[i] = __x;
            __x->_M_prev[i] = (_Node*)left_next[i];
        }
        
        return __x;
        
    }
    
    iterator _skipTo(const _Tp& __x) {
        _Node* _cur_node = (_Node*)this->_M_node;
        _Node* _next_node;
        int _curl = this->_maxlv - 1;
        
        while(_curl>0 && _cur_node->_M_next[_curl] == _cur_node)
            --_curl;
        if(_cur_node->_M_next[_curl] == _cur_node)
            return end();
        
        
        while(true){
            _next_node = (_Node*)_cur_node->_M_next[_curl];
            
            if(_next_node != (_Node*)this->_M_node  && _next_node->_M_data == __x){
                while(_next_node->_M_prev[0] != this->_M_node
                      && ((_Node*)_next_node->_M_prev[0])->_M_data == __x)
                    _next_node = (_Node*)_next_node->_M_prev[0];
                return iterator(_next_node);
            }else if(_next_node == (_Node*)this->_M_node || _next_node->_M_data > __x){
                if(_curl > 0)
                    --_curl;
                else
                    return iterator(_next_node);
            }else
                _cur_node = _next_node;
        }
        return end();
    }
    
    void _left_snapshot(iterator __left, iterator __pos, unsigned int _lv, vector<_Node*>& res)const{
        _Node* _cur = (_Node*)(--__pos)._M_node;
        unsigned int _curl = 0;
        res.clear();
        
        while(true){
            for(; _curl < _cur->_lv && _curl < _lv; ++_curl)
                res.push_back(_cur);
            _cur = (_Node*)_cur->_M_prev[0];
            
            while(true)
                if(_cur == (_Node*)__left._M_node->_M_prev[0] || _curl == _lv) return;
                else if(!(_cur->_lv > _curl))
                    _cur = (_Node*)_cur->_M_prev[0];
                else break;
        }
    }
    void _right_snapshot (iterator __pos, iterator __right, unsigned int _lv, vector<_Node*>& res)const{
        _Node* _cur = (_Node*)(__pos)._M_node;
        unsigned int _curl = 0;
        res.clear();
        
        while(true){
            for(; _curl < _cur->_lv && _curl < _lv; ++_curl)
                res.push_back(_cur);
            _cur = (_Node*)_cur->_M_next[0];
            
            while(true)
                if(_cur == (_Node*)__right._M_node || _curl == _lv) return;
                else if(!(_cur->_lv > _curl))
                    _cur = (_Node*)_cur->_M_next[0];
                else break;
        }
    }
    
public:
    
    void printlv(){
        for(auto iter = begin(); iter != end(); ++iter){
            cout<<iter._M_node->_lv<<" ";
        }
        cout<<endl;
    }
    void printdt(){
        for(auto iter = begin(); iter != end(); ++iter){
            cout<<*iter<<" ";
        }
        cout<<endl;
    }
    
    iterator begin()             { return (_Node*)(_M_node->_M_next[0]); }
    const_iterator begin() const { return (_Node*)(_M_node->_M_next[0]); }
    iterator end()             { return (_Node*)_M_node; }
    const_iterator end() const { return (_Node*)_M_node; }
    
    reverse_iterator rbegin() { return reverse_iterator(end()); }
    const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }
    reverse_iterator rend() { return reverse_iterator(begin()); }
    const_reverse_iterator rend() const { return const_reverse_iterator(begin()); }
    
    bool empty() const { return _M_node->_M_next[0] == _M_node; }
    size_type size() const {
        return (size_type)distance(begin(), end());
    }
    size_type max_size() const { return size_type(-1); }
    
    reference front() { return *begin(); }
    const_reference front() const { return *begin(); }
    reference back() { return *(--end()); }
    const_reference back() const { return *(--end()); }
    
    void swap(skiplist<_Tp>& __x) {
        
        auto tmpnode = this->_M_node;
        this->_M_node = __x._M_node;
        __x._M_node = tmpnode;
        
        auto tmplv = this->_maxlv;
        this->_maxlv = __x._maxlv;
        __x._maxlv = tmplv;
    }
    
protected:
    iterator _insert(iterator __pos, const _Tp& __x){
        _Node* __tmp = this->_M_get_node(__x);
        unsigned int _lv = policy->get_level(this, __pos);
        //cout<<"this is get level res: "<<_lv<<endl;
        _M_set_level(__tmp, _lv);
        _insert_level(__tmp, __pos);
        return __tmp;
    }
    void _insert(iterator __pos, size_type __n, const _Tp& __x){
        skiplist<_Node*> carry;
        for(size_type i = 0; i != __n; ++i)
            carry._insert(carry.end(), __x);
        this->transfer(__pos, carry.begin(), carry.end());
        return;
    }
    template <class _InputIterator>
    void _insert(iterator __pos, _InputIterator __first, _InputIterator __last){
        if(__first == __last) return;
        
        skiplist<typename iterator_traits<_InputIterator>::value_type> carry;
        _Tp last = *__first;
        do{
            if(*__first < last)
                return;
            carry._insert(carry.end(), *__first);
            last = *__first;
        }while(++__first != __last);
        this->transfer(__pos, carry.begin(), carry.end());
        return;
    }
    
public:
    iterator find(const _Tp& __x){
        return _skipTo(__x);
    }
    
    iterator insert(const _Tp& __x){
        return _insert(_skipTo(__x), __x);
    }
    void insert(size_type __n, const _Tp& __x){
        return _insert(_skipTo(__x), __n, __x);
    }
    template <class _InputIterator>
    void insert(_InputIterator __first, _InputIterator __last) {
        while(__first != __last)
            insert(*__first++);
        return;
    }
    
    
    iterator erase(iterator __position) {
        if(__position == end()) return __position;
        _p_skiplist_node_base __real_next = __position._M_node->_M_next[0];;
        _p_skiplist_node_base __next_node;
        _p_skiplist_node_base __prev_node;
        _Node* __n = (_Node*) __position._M_node;
        for(int i = 0; i != __position._M_node->_lv; ++i){
            __next_node = __n->_M_next[i];
            __prev_node = __n->_M_prev[i];
            __prev_node->_M_next[i] = __next_node;
            __next_node->_M_prev[i] = __prev_node;
        }
        _M_put_node(__n);
        return iterator((_Node*) __real_next);
    }
    iterator erase(const _Tp& __x) {
        iterator __pos = _skipTo(__x);
        if(__pos == end() || *__pos > __x)
            return end();
        else
            return erase(__pos);
    }
    iterator erase(iterator __first, iterator __last){
        while (__first != __last)
            erase(__first++);
        return __last;
    }
    
    void clear() { _Base::clear(); }
    
    void resize(size_type __new_size, const _Tp& __x){
        iterator __i = begin();
        size_type __len = 0;
        for ( ; __i != end() && __len < __new_size; ++__i, ++__len)
            ;
        if (__len == __new_size)
            erase(__i, end());
        else                          // __i == end()
            insert(__new_size - __len, __x);
    }
    void resize(size_type __new_size) {
        this->resize(__new_size, _Tp());
    }
    
    void pop_front() { erase(begin()); }
    void pop_back() {
        iterator __tmp = end();
        erase(--__tmp);
    }
    
    skiplist():_Base(){
        this->policy = new default_skip_policy();
        this->_maxlv = MaxLevel;
    }
    
    skiplist(_skip_policy* plc, unsigned int lv = MaxLevel):_Base(lv),policy(plc){}
    
    skiplist(size_type __n, const _Tp& __value){ _insert(begin(), __n, __value); }
    
    template <class _InputIterator>
    skiplist(_InputIterator __first, _InputIterator __last):skiplist(){
        is_sorted(__first, __last)==false?this->insert(__first, __last):this->_insert(__first,__last);
    }
    
    skiplist(const_iterator __first, const_iterator __last):skiplist(){
        is_sorted(__first, __last)==false?this->insert(__first, __last):this->_insert(__first,__last);
    }
    
    skiplist(const list<_Tp>& __x):skiplist(){
        is_sorted(__x.begin(), __x.end())==false?this->insert(__x.begin(), __x.end()):this->_insert(__x.begin(),__x.end()); }
    
    skiplist(const vector<_Tp>& __x):skiplist(){
        is_sorted(__x.begin(), __x.end())==false?this->insert(__x.begin(), __x.end()):this->_insert(__x.begin(),__x.end()); }
    
    skiplist(const skiplist<_Tp>& __x){
        this->policy = new default_skip_policy();
        this->_maxlv = __x._maxlv;
        _insert(begin(), __x.begin(), __x.end());
    }
    
    ~skiplist() { }
    
    skiplist<_Tp>& operator=(const skiplist<_Tp>& __x){
        if (this != &__x) {
            iterator __first1 = begin();
            iterator __last1 = end();
            const_iterator __first2 = __x.begin();
            const_iterator __last2 = __x.end();
            while (__first1 != __last1 && __first2 != __last2)
                *__first1++ = *__first2++;
            if (__first2 == __last2)
                erase(__first1, __last1);
            else
                insert(__last1, __first2, __last2);
        }
        return *this;
    }
    
    void assign(size_type __n, const _Tp& __val) { this->clear(); _insert(__n, __val); }
    
    template <class _InputIterator>
    void assign(_InputIterator __first, _InputIterator __last) {
        this->clear();
        insert(__first, __last);
    }
    
    void splice(iterator __position, skiplist& __x) {
        if (!__x.empty())
            this->transfer(__position, __x.begin(), __x.end());
    }
    void splice(iterator __position, skiplist&, iterator __i) {
        iterator __j = __i;
        ++__j;
        if (__position == __i || __position == __j) return;
        this->transfer(__position, __i, __j);
    }
    void splice(iterator __position, skiplist&, iterator __first, iterator __last) {
        if (__first != __last)
            this->transfer(__position, __first, __last);
    }
    
    
    void transfer(iterator __position, iterator __first, iterator __last){
        vector<_Node*> left_next;
        _left_snapshot(iterator(this->_M_node), __position, this->_maxlv, left_next);
        vector<_Node*> splice_prev;
        _right_snapshot(__first, __last, this->_maxlv, splice_prev);
        vector<_Node*> splice_next;
        _left_snapshot(__first, __last, this->_maxlv, splice_next);
        
        if(splice_next.size() != splice_prev.size()){
            cout<< "size is different"<<endl;
            exit(0);
        }
        
        left_next.resize(splice_next.size());
        for(unsigned int i = 0; i < splice_next.size(); ++i){
            splice_next[i]->_M_next[i]->_M_prev[i] = (_Node*)splice_prev[i]->_M_prev[i];
            splice_prev[i]->_M_prev[i]->_M_next[i] = (_Node*)splice_next[i]->_M_next[i];
            
            left_next[i]->_M_next[i]->_M_prev[i] = (_Node*)splice_next[i];
            splice_next[i]->_M_next[i] = left_next[i]->_M_next[i];
            
            left_next[i]->_M_next[i] = (_Node*)splice_prev[i];
            splice_prev[i]->_M_prev[i] = (_Node*)left_next[i];
        }
        return;
    }
    
    void remove(const _Tp& __value){
        iterator _cur = this->_skipTo(__value);
        if(_cur == end()) return;
        iterator _next = _cur;
        
        while(*_cur == __value){
            _cur = _next++;
            erase(_cur);
            _cur = _next;
        }
        return;
    }
    void unique(){
        iterator __first = begin();
        iterator __last = end();
        if (__first == __last) return;
        iterator __next = __first;
        while (++__next != __last) {
            if (*__first == *__next)
                erase(__next);
            else
                __first = __next;
            __next = __first;
        }
    }
    void merge(skiplist& __x){
        iterator __first1 = begin();
        iterator __last1 = end();
        iterator __first2 = __x.begin();
        iterator __last2 = __x.end();
        
        if(this->_maxlv < __x._maxlv){
            int _clv = this->_max_lv;
            int _tlv = __x->_maxlv;
            _Node* _header = (_Node*)this->_M_node;
            _p_skiplist_node_base* _new_next = new _p_skiplist_node_base[_tlv];
            _p_skiplist_node_base* _new_prev = new _p_skiplist_node_base[_tlv];
            
            for(unsigned int i = 0; i < _clv; ++i){
                _new_next[i] = (_Node*)_header->_M_next[i];
                _new_prev[i] = (_Node*)_header->_M_prev[i];
            }
            for(unsigned int i = _clv; i < _tlv; ++i){
                _new_next[i] = _header;
                _new_prev[i] = _header;
            }
            
            if(_clv > 0){
                delete[] _header->_M_next;
                delete[] _header->_M_prev;
            }
            
            _header->_M_next = _new_next;
            _header->_M_prev = _new_prev;
            _header->_lv = _tlv;
            this->_maxlv = _tlv;
        }
        
        while (__first1 != __last1 && __first2 != __last2)
            if (*__first2 < *__first1) {
                iterator __next = __first2;
                while(*__next < *__first1) ++__next;
                transfer(__first1, __first2, __next);
                __first2 = __next;
            }
            else
                ++__first1;
        if (__first2 != __last2) transfer(__last1, __first2, __last2);
    }
    void sort(){};
    void make_average(unsigned int shrink=5){
        typename average_skip_policy::carrier carrier(shrink, this->_maxlv);
        iterator __last = end();
        _Node* _cur_node = (_Node*)begin()._M_node;
        _Node* _next_node = (_Node*)begin()._M_node;
        unsigned int _curl = 0;
        vector<_Node*> a(this->_maxlv, (_Node*)this->_M_node);
        
        while(_cur_node != (_Node*)__last._M_node){
            _curl = carrier.add();
            _next_node = (_Node*)_cur_node->_M_next[0];
            
            if(_cur_node->_lv != _curl){
                delete[] _cur_node->_M_prev;
                delete[] _cur_node->_M_next;
                _M_set_level(_cur_node, _curl);
            }
            
            for(unsigned int i = 0; i < _curl; ++i){
                _cur_node->_M_prev[i] = a[i];
                a[i]->_M_next[i] = _cur_node;
                a[i] = (_Node*)_cur_node;
            }
            
            _cur_node = _next_node;
        }
        
        for(unsigned int i = 0; i < this->_maxlv; ++i){
            a[i]->_M_next[i] = __last._M_node;
            __last._M_node->_M_prev[i] = a[i];
        }
        
    }
    void make_Zipf(map<_Tp, unsigned int>& weight){
        
    }
    
    template <class _Predicate> void remove_if(_Predicate __pred){
        iterator __first = begin();
        iterator __last = end();
        while (__first != __last) {
            iterator __next = __first;
            ++__next;
            if (__pred(*__first)) erase(__first);
            __first = __next;
        }
    }
    template <class _BinaryPredicate> void unique(_BinaryPredicate __binary_pred){
        iterator __first = begin();
        iterator __last = end();
        if (__first == __last) return;
        iterator __next = __first;
        while (++__next != __last) {
            if (__binary_pred(*__first, *__next))
                erase(__next);
            else
                __first = __next;
            __next = __first;
        }
    }
    template <class _StrictWeakOrdering> void merge(skiplist<_Tp>& __x,
                                                    _StrictWeakOrdering __comp){
        iterator __first1 = begin();
        iterator __last1 = end();
        iterator __first2 = __x.begin();
        iterator __last2 = __x.end();
        
        if(this->_maxlv < __x._maxlv){
            int _clv = this->_maxlv;
            int _tlv = __x._maxlv;
            _Node* _header = (_Node*)this->_M_node;
            _p_skiplist_node_base* _new_next = new _p_skiplist_node_base[_tlv];
            _p_skiplist_node_base* _new_prev = new _p_skiplist_node_base[_tlv];
            
            for(unsigned int i = 0; i < _clv; ++i){
                _new_next[i] = (_Node*)_header->_M_next[i];
                _new_prev[i] = (_Node*)_header->_M_prev[i];
            }
            for(unsigned int i = _clv; i < _tlv; ++i){
                _new_next[i] = _header;
                _new_prev[i] = _header;
            }
            
            if(_clv > 0){
                delete[] _header->_M_next;
                delete[] _header->_M_prev;
            }
            
            _header->_M_next = _new_next;
            _header->_M_prev = _new_prev;
            _header->_lv = _tlv;
            this->_maxlv = _tlv;
        }
        
        while (__first1 != __last1 && __first2 != __last2)
            if (__comp(*__first2, *__first1)) {
                iterator __next = __first2;
                while(__next !=__last2 && __comp(*__next, *__first1))
                    ++__next;
                this->transfer(__first1, __first2, __next);
                __first2 = __next;
            }
            else
                ++__first1;
        if (__first2 != __last2) transfer(__last1, __first2, __last2);
    }
    template <class _StrictWeakOrdering> list<_Tp> sort(_StrictWeakOrdering __comp){
        list<_Tp> res;
        if (_M_node->_M_next[0] != _M_node && _M_node->_M_next[0]->_M_next[0] != _M_node) {
            list<_Tp> __carry;
            list<_Tp> __counter[64];
            int __fill = 0;
            while (!empty()) {
                __carry.splice(__carry.begin(), *this, begin());
                int __i = 0;
                while(__i < __fill && !__counter[__i].empty()) {
                    __counter[__i].merge(__carry, __comp);
                    __carry.swap(__counter[__i++]);
                }
                __carry.swap(__counter[__i]);
                if (__i == __fill) ++__fill;
            }
            
            for (int __i = 1; __i < __fill; ++__i)
                __counter[__i].merge(__counter[__i-1], __comp);
            
            for(typename list<_Tp>::iterator iter = __counter[__fill-1].begin();
                iter != __counter[__fill-1].end(); ++iter)
                res.push_back(*iter);
            
        }else if(_M_node->_M_next[0]->_M_next[0] == _M_node){
            res.push_back(((_Node*)_M_node->_M_next[0])->_M_data);
        }
        
        return res;
    }
    
};

template <class _Tp>
inline bool
operator==(const skiplist<_Tp>& __x, const skiplist<_Tp>& __y)
{
    typedef typename skiplist<_Tp>::const_iterator const_iterator;
    const_iterator __end1 = __x.end();
    const_iterator __end2 = __y.end();
    
    const_iterator __i1 = __x.begin();
    const_iterator __i2 = __y.begin();
    while (__i1 != __end1 && __i2 != __end2 && *__i1 == *__i2) {
        ++__i1;
        ++__i2;
    }
    return __i1 == __end1 && __i2 == __end2;
}

template <class _Tp>
inline bool operator<(const skiplist<_Tp>& __x,
                      const skiplist<_Tp>& __y)
{
    return lexicographical_compare(__x.begin(), __x.end(),
                                   __y.begin(), __y.end());
}

template <class _Tp>
inline bool operator!=(const skiplist<_Tp>& __x,
                       const skiplist<_Tp>& __y) {
    return !(__x == __y);
}

template <class _Tp>
inline bool operator>(const skiplist<_Tp>& __x,
                      const skiplist<_Tp>& __y) {
    return __y < __x;
}

template <class _Tp>
inline bool operator<=(const skiplist<_Tp>& __x,
                       const skiplist<_Tp>& __y) {
    return !(__y < __x);
}

template <class _Tp>
inline bool operator>=(const skiplist<_Tp>& __x,
                       const skiplist<_Tp>& __y) {
    return !(__x < __y);
}

template <class _Tp>
inline void
swap(skiplist<_Tp>& __x, skiplist<_Tp>& __y)
{
    __x.swap(__y);
}


template <class _Tp>
list<_Tp> intersect(skiplist<_Tp>& __x, skiplist<_Tp>& __y){
    
}


#endif /* skiplist_h */
