// Brain Party
// Copyright (C) 2010 Paul Hudson (http://www.tuxradar.com/brainparty)

// Brain Party is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 3
// of the License, or (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

#ifndef __BPList_H__
#define __BPList_H__

#include <vector>
#include <algorithm>

#include "Texture.h"
#include "Colour.h"
#include "BPPoint.h"

using namespace std;

template <class T>
class BPList {
private:
	vector<T> data;
	
public:
	int Count;
	BPList();
	~BPList();
	void Add(T item);
	bool Contains(T item);
	void Clear();
	int IndexOf(T item);
	void Insert(int pos, T item);
	void Remove(T item);
	void RemoveAt(int pos);
	void SetValue(int pos, T val);
	void Shuffle();
	T operator[] (int position);
};

template <class T>
BPList<T>::BPList() {
	Count = 0;
}

template <class T>
BPList<T>::~BPList() {	
	
}

template <class T>
void BPList<T>::Add(T item) {
	data.push_back(item);
	++Count;
}

template <class T>
bool BPList<T>::Contains(T item) {
	typename std::vector<T>::iterator result = find(data.begin(), data.end(), item);
	return result != data.end();
}

template <class T>
void BPList<T>::Clear() {
	data.clear();
	Count = 0;
}

template <class T>
T BPList<T>::operator[] (int position) {
	return data[position];
}

template <class T>
void BPList<T>::Insert(int pos, T item) {
	data.insert(data.begin() + pos, item);
	++Count;
}

template <class T>
void BPList<T>::Shuffle() {
	random_shuffle(data.begin(), data.end());
}

template <class T>
void BPList<T>::Remove(T item) {
	typename std::vector<T>::iterator result = find(data.begin(), data.end(), item);
	if (result != data.end()) {
		data.erase(result);
		--Count;
	}
}

template <class T>
void BPList<T>::RemoveAt(int pos) {
	data.erase(data.begin() + pos);
	--Count;
}

template <class T>
int BPList<T>::IndexOf(T item) {
	typename std::vector<T>::iterator result = find(data.begin(), data.end(), item);
	
	if (result == data.end()) return -1;
	
	return result - data.begin();
}

template <class T>
void BPList<T>::SetValue(int pos, T val) {
	data[pos] = val; 
}







template <class T>
class BPPList {
private:
	vector<T> data;
	
public:
	int Count;
	BPPList();
	~BPPList();
	void Add(T item);
	bool Contains(T item);
	void Clear(bool safe_delete=true);
	int IndexOf(T item);
	void Insert(int pos, T item);
	void Remove(T item);
	void RemoveAt(int pos);
	void SetValue(int pos, T val);
	void Shuffle();
	T operator[] (int position);
};

template <class T>
BPPList<T>::BPPList() {
	Count = 0;
}

template <class T>
BPPList<T>::~BPPList() {	

}

template <class T>
void BPPList<T>::Add(T item) {
	data.push_back(item);
	++Count;
}

template <class T>
bool BPPList<T>::Contains(T item) {
	typename std::vector<T>::iterator result = find(data.begin(), data.end(), item);
	return result != data.end();
}

template <class T>
void BPPList<T>::Clear(bool safe_delete) {
	if (safe_delete) {
		for (int i = data.size() - 1; i >= 0; --i) {
			T item = data[i];
			SAFE_DELETE(item);
		}
	}
	
	data.clear();
	Count = 0;
}

template <class T>
T BPPList<T>::operator[] (int position) {
	return data[position];
}

template <class T>
void BPPList<T>::Insert(int pos, T item) {
	data.insert(data.begin() + pos, item);
	++Count;
}

template <class T>
void BPPList<T>::Shuffle() {
	random_shuffle(data.begin(), data.end());
}

template <class T>
void BPPList<T>::Remove(T item) {
	typename std::vector<T>::iterator result = find(data.begin(), data.end(), item);
	if (result != data.end()) {
		data.erase(result);
		--Count;
	}
}

template <class T>
void BPPList<T>::RemoveAt(int pos) {
	data.erase(data.begin() + pos);
	--Count;
}

template <class T>
int BPPList<T>::IndexOf(T item) {
	typename std::vector<T>::iterator result = find(data.begin(), data.end(), item);
	
	if (result == data.end()) return -1;
	
	return result - data.begin();
}

template <class T>
void BPPList<T>::SetValue(int pos, T val) {
	data[pos] = val; 
}

#endif
