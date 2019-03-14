#pragma once
#include <boost/asio.hpp>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/composite_key.hpp>
using namespace boost::asio;

template<typename T>
void do_release(typename boost::shared_ptr<T> const&, T*)
{
}

template<typename T>
typename std::shared_ptr<T> to_std(typename boost::shared_ptr<T> const& p)
{
	return
		std::shared_ptr<T>(
			p.get(),
			boost::bind(&do_release<T>, p, _1));

}

class boostClient
{
	io_service service;
public:
	boostClient();
	~boostClient();
};

