
/*
 * Copyright (c) 2001 Institute for Information Industry, Taiwan, Republic of China 
 * (http://www.iii.org.tw/iiia/ewelcome.htm)
 *
 * Permission to copy, use, modify, sell and distribute this software
 * is granted provided this copyright notice appears in all copies.
 * This software is provided "as is" without express or implied
 * warranty, and with no claim as to its suitability for any purpose.
 */

#ifndef IOS_HELPER_H
#define IOS_HELPER_H
#include <ios>

/*
 * The following code is adapted from the book "Standard C++ IOStreams and Locales, by
 *  Angelika Langer and Klaus Kreft, ISBN 0-201-18395-1, pp 170~172".
 */

template <class charT, class Traits>
class IOWrapper
{
public:
	void do_it(std::basic_ios<charT, Traits>& ios);
private:
	virtual std::ios_base::iostate do_real_io() =0;
};

template <class charT, class Traits>
void IOWrapper<charT, Traits>::do_it(std::basic_ios<charT, Traits>& ios)
{
	std::ios_base::iostate err ;
	try {
		err = do_real_io();
	}
	catch (std::bad_alloc&)
	{
		err |= std::ios_base::badbit;
		std::ios_base::iostate exception_mask = ios.exceptions();
		if ((exception_mask & std::ios_base::failbit) &&
			!(exception_mask & std::ios_base::badbit) )
		{ ios.setstate(err); }
		else if (exception_mask & std::ios_base::badbit)
		{  
			try {	ios.setstate(err);  }
			catch (std::ios_base::failure& ) {}
			throw;
		}
	}
	catch (...)
	{
		err |= std::ios_base::failbit;
		std::ios_base::iostate exception_mask = ios.exceptions();
		if ((exception_mask & std::ios_base::badbit) &&
			(err & std::ios_base::badbit) )
		{ ios.setstate(err); }
		else if (exception_mask & std::ios_base::failbit)
		{  
			try {	ios.setstate(err);  }
			catch (std::ios_base::failure& ) {}
			throw;
		}	
	}
	if (err) ios.setstate(err);
}

template <class charT, class Traits, class Argument>
class ExtractorWrapper : public IOWrapper<charT, Traits>
{
public:
	ExtractorWrapper(std::basic_istream<charT, Traits>& istrm, Argument& argument) :is(istrm), arg(argument){}
private:
	std::ios_base::iostate do_real_io()
	{
		typename std::basic_istream<charT, Traits>::sentry ipfs(is);
		if (ipfs)
			return arg.get_from(is);
		else
			return std::ios_base::goodbit;
	}
	std::basic_istream<charT, Traits>& is;
	Argument& arg;
};

template <class charT, class Traits, class Argument>
class InserterWrapper : public IOWrapper<charT, Traits>
{
public:
	InserterWrapper(std::basic_ostream<charT, Traits>& ostrm, const Argument& argument) :os(ostrm), arg(argument){}
private:
	std::ios_base::iostate do_real_io()
	{
		typename std::basic_ostream<charT, Traits>::sentry opfs(os);
		if (opfs)
			return arg.print_on(os);
		else
			return std::ios_base::goodbit;
	}
	std::basic_ostream<charT, Traits>& os;
	const Argument& arg;
};

template <class charT, class Traits, class Argument>
std::basic_istream<charT, Traits>& g_extractor (std::basic_istream<charT, Traits>& is, Argument& arg)
{
	if (is.good())
	{
		ExtractorWrapper<charT, Traits, Argument> extr(is,arg);
		extr.do_it(is);
	}
	return is;
}

template <class charT, class Traits, class Argument>
std::basic_ostream<charT, Traits>& g_inserter (std::basic_ostream<charT, Traits>& os, const Argument& arg)
{
	if (os.good())
	{
		InserterWrapper<charT, Traits, Argument> intr(os, arg);
		intr.do_it(os);
	}
	return os;
}

#endif
