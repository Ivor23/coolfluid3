// Copyright (C) 2010 von Karman Institute for Fluid Dynamics, Belgium
//
// This software is distributed under the terms of the
// GNU Lesser General Public License version 3 (LGPLv3).
// See doc/lgpl.txt and doc/gpl.txt for the license text.

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE "Test module for parallel fields"

#include <iomanip>
#include <boost/test/unit_test.hpp>

#include "Common/Log.hpp"
#include "Common/Core.hpp"
#include "Common/CRoot.hpp"
#include "Common/CEnv.hpp"

#include "Common/Foreach.hpp"
#include "Common/OSystem.hpp"
#include "Common/OSystemLayer.hpp"

#include "Common/MPI/PE.hpp"
#include "Common/MPI/Buffer.hpp"
#include "Common/MPI/debug.hpp"

using namespace boost;
using namespace CF;
using namespace CF::Common;
using namespace CF::Common::mpi;

////////////////////////////////////////////////////////////////////////////////

struct MPIBufferTests_Fixture
{
  /// common setup for each test case
  MPIBufferTests_Fixture()
  {
    // uncomment if you want to use arguments to the test executable
    m_argc = boost::unit_test::framework::master_test_suite().argc;
    m_argv = boost::unit_test::framework::master_test_suite().argv;

  }

  /// common tear-down for each test case
  ~MPIBufferTests_Fixture()
  {
  }

  /// possibly common functions used on the tests below
  int m_argc;
  char** m_argv;
};

////////////////////////////////////////////////////////////////////////////////

BOOST_FIXTURE_TEST_SUITE( MPIBufferTests_TestSuite, MPIBufferTests_Fixture )

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE( init_mpi )
{
  Core::instance().initiate(m_argc,m_argv);
  mpi::PE::instance().init(m_argc,m_argv);
}

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE( test_MPIBuffer )
{

  mpi::Buffer buf;
  buf << 1u << 2u << 3.0 << 4 << true;
  BOOST_CHECK_EQUAL( buf.more_to_unpack(), true);

  Uint data_unsigned;
  Real data_real;
  int data_integer;
  std::vector<Real> data_array_real;
  RealVector data_realvector;
  std::string data_string;
  std::vector<std::string> data_array_string;
  bool data_bool;

  buf >> data_unsigned;  BOOST_CHECK_EQUAL(data_unsigned, 1u  );
  buf >> data_unsigned;  BOOST_CHECK_EQUAL(data_unsigned, 2u  );
  buf >> data_real;      BOOST_CHECK_EQUAL(data_real,     3.0 );
  buf >> data_integer;   BOOST_CHECK_EQUAL(data_integer,  4   );
  buf >> data_bool;      BOOST_CHECK_EQUAL(data_bool,     true);
  BOOST_CHECK_EQUAL( buf.more_to_unpack(), false);

  buf << 5;  buf >> data_integer; BOOST_CHECK_EQUAL (data_integer , 5);
  BOOST_CHECK_EQUAL( buf.more_to_unpack(), false);

  buf << std::vector<Real>(4,6.);
  buf >> data_array_real;
  BOOST_CHECK_EQUAL(data_array_real.size(), 4u);
  BOOST_CHECK_EQUAL(data_array_real[0], 6.);
  BOOST_CHECK_EQUAL(data_array_real[1], 6.);
  BOOST_CHECK_EQUAL(data_array_real[2], 6.);
  BOOST_CHECK_EQUAL(data_array_real[3], 6.);
  BOOST_CHECK_EQUAL( buf.more_to_unpack(), false);

  boost::multi_array<Real,2> table;
  table.resize(boost::extents[4][3]);
  table[2][0]=1.;
  table[2][1]=2.;
  table[2][2]=3.;

  buf << table[2];
  buf >> data_array_real;
  BOOST_CHECK_EQUAL(data_array_real[0], 1.);
  BOOST_CHECK_EQUAL(data_array_real[1], 2.);
  BOOST_CHECK_EQUAL(data_array_real[2], 3.);
  BOOST_CHECK_EQUAL( buf.more_to_unpack(), false);

  buf << table[2];
  buf >> data_realvector;
  BOOST_CHECK_EQUAL(data_realvector[0], 1.);
  BOOST_CHECK_EQUAL(data_realvector[1], 2.);
  BOOST_CHECK_EQUAL(data_realvector[2], 3.);
  BOOST_CHECK_EQUAL( buf.more_to_unpack(), false);

  buf << std::string("a nice string");
  buf >> data_string;
  BOOST_CHECK_EQUAL(data_string, std::string("a nice string"));
  BOOST_CHECK_EQUAL(buf.more_to_unpack(), false);

  buf << std::vector<std::string>(3,"another nice string");
  buf >> data_array_string;
  BOOST_CHECK_EQUAL(data_array_string[0], std::string("another nice string"));
  BOOST_CHECK_EQUAL(data_array_string[1], std::string("another nice string"));
  BOOST_CHECK_EQUAL(data_array_string[2], std::string("another nice string"));
  BOOST_CHECK_EQUAL(buf.more_to_unpack(), false);

}

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE( finalize_mpi )
{
  mpi::PE::instance().finalize();
  Core::instance().terminate();
}

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_SUITE_END()

////////////////////////////////////////////////////////////////////////////////
