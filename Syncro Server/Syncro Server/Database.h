#ifndef __DATABASE_H_
#define __DATABASE_H_

//#include "Logger.h"
//extern CLogger Logger;

#include <string>
#include <sqlite3.h>

#include <vector>
#include <map>

#include <time.h>

#ifdef USING_PTHREADS
#include <pthread.h>
#endif

namespace syncro {

using namespace std;

class Database
{
public:
	typedef map<string,string> Row;
	class ResultSet
	{
	protected:
		vector<Row> rows;
		vector<string> colNames;
	public:
		Database::Row operator[](int ID){return rows[ID];};
		Database::Row getRow(int ID) {return rows[ID];};
		int numRows() {return rows.size();};
		int numCols() {return colNames.size();};
		void clear() { rows.clear(); colNames.clear(); };
		void add(Database::Row row) { rows.push_back(row); };
		void defineCols(int num,char **names)
		{
			for(int i =0;i<num;i++)
				colNames.push_back(string(names[i]));
		}
		Database::Row find(string col,string value)
		{
			for(unsigned int i=0;i<rows.size();i++)
			{
				if(rows[i][col] == value)
					return rows[i];
			}
			return Row();
		}
		void log(int level)
		{
			if(numRows() != 0)
			{
				string names = "Columns:";
				for(unsigned int i=0;i<colNames.size();i++)
					names += colNames[i] + "|";
//				Logger.Log(names,level);
				for(unsigned int i=0;i<rows.size();i++)
				{
					string values = "";
					for(unsigned int y=0;y<colNames.size();y++)
						values += rows[i][colNames[y]] + "|";
					//Logger.Log(values,level);
				}
			}
		}
	};
	
	friend int callback(void*,int,char**,char**);

	Database(string file);
	~Database();

	ResultSet run(string query);

private:
	sqlite3 *db;
	ResultSet result;

	void clearResult();

#ifdef USING_PTHREADS
	pthread_mutex_t mutex;
#endif
};

}; //namespace syncro

#endif
