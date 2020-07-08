#ifndef _JSON_OBJECTBASE_HEADER_
#define _JSON_OBJECTBASE_HEADER_

#if defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#elif defined(_MSC_VER)
#pragma warning(disable : 4996)
#endif

using std::string;
using std::vector;

#define strName(str) #str
#define property(value) strName(value), typeid(value).name(), &value

class client_sokt_t : public uv_tcp_t
{
public:
	uint64_t ulPkgIndex;
};

class CJsonObjectBase
{
public:
	virtual ~CJsonObjectBase(){};
	virtual CJsonObjectBase *requestOperation() = 0;
	virtual string getResponse(){};
public:
	string Serialize()
	{
		Json::Value new_item;
		int nSize = m_listName.size();
		for (int i = 0; i < nSize; ++i)
		{
			void *pAddr = m_listPropertyAddr[i];
			if (m_listType[i] == typeid(int).name())
			{
				new_item[m_listName[i]] = (*(int *)pAddr);
			}
			else if (m_listType[i] == typeid(uint32_t).name())
			{
				new_item[m_listName[i]] = (*(uint32_t *)pAddr);
			}
			else if (m_listType[i] == typeid(int64_t).name())
			{
				new_item[m_listName[i]] = (*(int64_t *)pAddr);
			}
			else if (m_listType[i] == typeid(uint64_t).name())
			{
				new_item[m_listName[i]] = (*(uint64_t *)pAddr);
			}
			else if (m_listType[i] == typeid(string).name())
			{
				new_item[m_listName[i]] = (*(string *)pAddr);
			}
			else
			{
			}
		}
		Json::FastWriter writer;
		std::string out2 = writer.write(new_item);
		return out2;
	}

	bool DeSerialize(const char *str)
	{
		Json::Reader reader;
		Json::Value root;
		if (reader.parse(str, root))
		{
			int nSize = m_listName.size();
			for (int i = 0; i < nSize; ++i)
			{
				void *pAddr = m_listPropertyAddr[i];
				if (m_listType[i] == typeid(int).name())
				{
					(*(int *)pAddr) = root.get(m_listName[i], 0).asInt();
				}
				else if (m_listType[i] == typeid(uint32_t).name())
				{
					(*(uint32_t *)pAddr) = root.get(m_listName[i], 0).asUInt();
				}
				else if (m_listType[i] == typeid(int64_t).name())
				{
					(*(int64_t *)pAddr) = root.get(m_listName[i], 0).asInt64();
				}
				else if (m_listType[i] == typeid(uint64_t).name())
				{
					(*(uint64_t *)pAddr) = root.get(m_listName[i], 0).asUInt64();
				}
				else if (m_listType[i] == typeid(string).name())
				{
					(*(string *)pAddr) = root.get(m_listName[i], "").asString();
				}
				else
				{
				}
			}
			return true;
		}
		return false;
	}

protected:
	void SetProperty(string name, string type, void *addr)
	{
		m_listName.push_back(name);
		m_listPropertyAddr.push_back(addr);
		m_listType.push_back(type);
	}
	vector<string> m_listName;
	vector<void *> m_listPropertyAddr;
	vector<string> m_listType;

public:
	client_sokt_t *m_client;
};
#endif