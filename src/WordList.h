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

#ifndef __WORDLIST_H__
#define __WORDLIST_H__

#include <vector>
#include <ext/hash_set>

using namespace std;
using namespace __gnu_cxx;

namespace __gnu_cxx
{
	template<> struct hash< std::string >
	{
		size_t operator()( const std::string& x ) const
		{
			return hash< const char* >()( x.c_str() );
		}
	};
}

typedef hash_set<string, hash<string> > string_hash_set;

class WordList {
private:
	string_hash_set Words;

public:
	WordList(const std::string &file) {
		FILE *fp;
		char filebuffer[255];
		
		if ((fp = fopen(file.c_str(), "r")) != NULL) {
			
			while(!feof(fp)) {
				fgets(filebuffer, 255, fp);
				char* word = new char[255];
				sscanf(filebuffer, "%s", word);
				Words.insert(word);
				delete[] word;
			}
			
			fclose(fp);
		}
	}
	
	~WordList() {
		Words.clear();
	}
	
	bool Contains(std::string string) {
		string_hash_set::const_iterator it = Words.find(string);
	
		if (it != Words.end()) {
			return true;
		} else {
			return false;
		}
	}
};

#endif
