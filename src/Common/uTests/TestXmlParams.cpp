#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include "Common/Log.hpp"
#include "Common/XmlHelpers.hpp"
#include "Common/BasicExceptions.hpp"

using namespace std;
using namespace boost;
using namespace CF;
using namespace CF::Common;

/////////////////////////////////////////////////////////////////////////////////////

struct XmlParams_Fixture
{
  /// common setup for each test case
  XmlParams_Fixture()
  {
    // int*    argc = &boost::unit_test::framework::master_test_suite().argc;
    // char*** argv = &boost::unit_test::framework::master_test_suite().argv;
  }

  /// common tear-down for each test case
  ~XmlParams_Fixture()
  {
  }

  /// possibly common functions used on the tests below

  /// common values accessed by all tests goes here

};

/////////////////////////////////////////////////////////////////////////////////////

BOOST_FIXTURE_TEST_SUITE( XmlParams_TestSuite, XmlParams_Fixture )

/////////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE( constructors )
{
  std::string text = (
      "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
      "<cfxml version=\"1.0\">"
      "<signal>"
      " <valuemap>"
      "  <value key=\"options\">"
      "   <valuemap>"
      "    <value  key=\"OptBool\"> <bool> true </bool> </value>"
      "    <value  key=\"OptInt\" > <integer> -156 </integer> </value>"
      "    <value  key=\"OptUint\" > <unsigned> 134 </unsigned> </value>"
      "    <value  key=\"OptReal\" > <real> 6.4564E+5 </real> </value>"
      "    <value  key=\"OptStr\" > <string> lolo </string> </value>"
      "    <component  key=\"OptComp\" >"
      "     <value key=\"name\"> <string> MyNewton </string> </value>"
      "     <value key=\"atype\"> <string> CIterativeMethod </string> </value>"
      "     <value key=\"ctype\"> <string> Newton </string> </value>"
      "    </component>"
      "   </valuemap>"
      "  </value>"
      " </valuemap>"
      "</signal>"
      "</cfxml>"
   );

  boost::shared_ptr<XmlDoc> xmldoc = XmlOps::parse ( text );

  XmlNode* docnode = XmlOps::goto_doc_node(*xmldoc.get());
  BOOST_CHECK_NO_THROW( XmlParams params ( *docnode->first_node() ) );

}

/////////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE( goto_doc_node )
{
  std::string text = (
      "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
   );

  boost::shared_ptr<XmlDoc> xmldoc = XmlOps::parse ( text );
  BOOST_CHECK_THROW(  XmlOps::goto_doc_node(*xmldoc.get()) , Common::XmlError );
}

/////////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE( throw_get_option )
{
  std::string text = (
      "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
      "<cfxml version=\"1.0\">"
      "</cfxml>"
   );

  boost::shared_ptr<XmlDoc> xmldoc = XmlOps::parse ( text );
  XmlNode& nodedoc = *XmlOps::goto_doc_node(*xmldoc.get());
  XmlParams params ( nodedoc );

  BOOST_CHECK_THROW( params.get_option<int>("noint")  , Common::XmlError );
}

/////////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE( get_option )
{
  std::string text = (
      "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
      "<cfxml version=\"1.0\">"
      "<signal>"
      " <valuemap>"
      "  <value key=\"options\">"
      "   <valuemap>"
      "    <value  key=\"OptBool\"> <bool> true </bool> </value>"
      "    <value  key=\"OptInt\" > <integer> -156 </integer> </value>"
      "    <value  key=\"OptUint\" > <unsigned> 134 </unsigned> </value>"
      "    <value  key=\"OptReal\" > <real> 6.4564E+5 </real> </value>"
      "    <value  key=\"OptStr\" > <string> lolo </string> </value>"
      "    <component  key=\"OptComp\" >"
      "     <value key=\"name\"> <string> MyNewton </string> </value>"
      "     <value key=\"atype\"> <string> CIterativeMethod </string> </value>"
      "     <value key=\"ctype\"> <string> Newton </string> </value>"
      "    </component>"
      "   </valuemap>"
      "  </value>"
      " </valuemap>"
      "</signal>"
      "</cfxml>"
   );

  boost::shared_ptr<XmlDoc> xmldoc = XmlOps::parse ( text );

  XmlNode& nodedoc = *XmlOps::goto_doc_node(*xmldoc.get());

  XmlParams params ( *nodedoc.first_node() );

//  XmlOps::print_xml_node(*doc.get());

  BOOST_REQUIRE_EQUAL ( params.get_option<bool>("OptBool") , true );

  BOOST_REQUIRE_EQUAL ( params.get_option<int>("OptInt"), -156 );

  BOOST_REQUIRE_EQUAL ( params.get_option<std::string>("OptStr") , "lolo" );

//  std::vector<Uint> v;
//  v += 2,8,9;
//  std::vector<Uint> cv = params.get_option<Uint>("VecInt");
//  BOOST_CHECK_EQUAL_COLLECTIONS( v.begin(), v.end(), cv.begin(), cv.end() );

  /// @todo how to access the nexted params?

//  BOOST_REQUIRE_EQUAL ( params.get_option<std::string>("OptComp") , "CGroup" );

}

/////////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_SUITE_END()

/////////////////////////////////////////////////////////////////////////////////////
