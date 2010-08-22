#ifndef bsc_cmd_hpp
#define bsc_cmd_hpp

#include <iostream>
#include <arpa/inet.h>

using namespace std;


inline void write_network_uint16(ostream &os, uint16_t i)
{
	uint16_t i_net = htons(i);
	os.write((char *)&i_net, sizeof(uint16_t));
}

inline void read_network_uint16(istream &is, uint16_t *i)
{
	is.read((char *)i, sizeof(uint16_t));
	*i = ntohs(*i);
}


enum bsc_cmd_types {
	BSC_RESERVE_REQ = 5,
	BSC_RESERVE_RES,
};



class bsc_cmd 
{
	public:
	uint16_t type;

	void serialize(ostream& os)
	{
		write_network_uint16(os, get_class_type());
		serialize_me(os);
	}

	bsc_cmd *unserialize(istream& is);

	virtual uint16_t get_class_type(void) { cout << "get_class_type" << endl; return 0; }

	protected:
	virtual void serialize_me(ostream&)	{ cout << "serialize_me" << endl; }
		
};

class bsc_reserve_req : public bsc_cmd 
{
	public:
	uint16_t id;

	bsc_reserve_req() {}

	bsc_reserve_req(istream& is)
	{
		read_network_uint16(is, &id);
	}

	void serialize_me(ostream& os)
	{
		write_network_uint16(os, id);
	}

	uint16_t get_class_type(void)
	{
		return BSC_RESERVE_REQ;
	}

};

class bsc_reserve_res : public bsc_cmd 
{
	public:
	uint16_t id;
	uint16_t data;

	bsc_reserve_res() {}

	bsc_reserve_res(istream& is)
	{
		read_network_uint16(is, &id);
		read_network_uint16(is, &data);
	}

	void serialize_me(ostream& os)
	{
		write_network_uint16(os, id);
		write_network_uint16(os, data);
	}

	uint16_t get_class_type(void)
	{
		return BSC_RESERVE_RES;
	}

};

bsc_cmd *bsc_cmd::unserialize(istream& is)
{
	read_network_uint16(is, &type);

	switch (type) 
	{
		case BSC_RESERVE_REQ:
			return new bsc_reserve_req(is);
			break;
		case BSC_RESERVE_RES:
			return new bsc_reserve_res(is);
			break;
		default:
			throw 5;
	
	}
}

#endif
