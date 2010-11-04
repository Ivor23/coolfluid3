// Copyright (C) 2010 von Karman Institute for Fluid Dynamics, Belgium
//
// This software is distributed under the terms of the
// GNU Lesser General Public License version 3 (LGPLv3).
// See doc/lgpl.txt and doc/gpl.txt for the license text.

#ifndef CF_Actions_ProtoElementLooper_hpp
#define CF_Actions_ProtoElementLooper_hpp

#include <boost/fusion/algorithm.hpp>
#include <boost/fusion/include/algorithm.hpp>
#include <boost/fusion/adapted/mpl.hpp>
#include <boost/fusion/include/mpl.hpp>
#include <boost/fusion/mpl.hpp>
#include <boost/fusion/container/vector/convert.hpp>
#include <boost/fusion/include/as_vector.hpp>
#include <boost/fusion/sequence.hpp>

#include <boost/mpl/for_each.hpp>
#include <boost/mpl/range_c.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/vector_c.hpp>

#include "Actions/Proto/ProtoContexts.hpp"

#include "Mesh/CMesh.hpp"

namespace CF {
namespace Actions {
namespace Proto {

////////////////////////////////////////////////////////
// Helper transforms
///////////////////////////////////////////////////////

/// Wrap up a var type in its context
template<typename SF, typename VarT>
struct AddContext
{
  typedef ElementVarContext<SF, VarT> type;
};

/// Initialize the contexts for numbered variables. Called when the CElements has changed
template<typename MappedCoordsT, typename VarsT, typename ContextsT>
struct InitContexts
{
  InitContexts(const VarsT& vars, ContextsT& contexts, Mesh::CElements& elements, const MappedCoordsT& mapped_coordinates) : m_vars(vars), m_contexts(contexts), m_elements(elements), m_mapped_coords(mapped_coordinates) {}
  
  template<typename I>
  void operator()(const I&)
  {
    boost::fusion::at<I>(m_contexts).init(boost::fusion::at<I>(m_vars), m_elements, m_mapped_coords);
  }
  
  const VarsT& m_vars;
  ContextsT& m_contexts;
  Mesh::CElements& m_elements;
  const MappedCoordsT& m_mapped_coords;
};

/// Fill the numbered variables. Called when the element changed
template<typename VarsT, typename ContextsT>
struct FillContexts
{
  FillContexts(const VarsT& vars, ContextsT& contexts) : m_vars(vars), m_contexts(contexts), elem_idx(0) {}
  
  template<typename I>
  void operator()(const I&)
  {
    boost::fusion::at<I>(m_contexts).fill(elem_idx);
  }
  
  const VarsT& m_vars;
  ContextsT& m_contexts;
  Uint elem_idx;
};

////////////////////////////////////////////////////////
// MPL for_each functor
///////////////////////////////////////////////////////

/// Iterate over the shape function types, so we can create shapefunction-specific contexts
template<typename ExpressionT, typename VarTypesT, Uint nb_vars>
class ElementLooper
{
public: // functions

  /// Constructor
  ElementLooper(const ExpressionT& expr, const VarTypesT& vars, Mesh::CElements& elements) : m_expr(expr), m_vars(vars), m_elements(elements) {}

  /// Operator
  template < typename ShapeFunctionT >
  void operator() ( ShapeFunctionT& T )
  {
    typedef typename ShapeFunctionT::MappedCoordsT MappedCoordsT;
    
    if(!Mesh::IsElementType<ShapeFunctionT>()(m_elements.element_type()))
      return;
    
    // Inititalize variable-specific contexts
    typedef typename boost::mpl::transform< VarTypesT, AddContext<ShapeFunctionT, boost::mpl::_1> >::type ContextsT;
    ContextsT contexts;
    
    MappedCoordsT mapped_coordinates;
    mapped_coordinates.setZero();
    
    InitContexts<MappedCoordsT, VarTypesT, ContextsT> init_ctx(m_vars, contexts, m_elements, mapped_coordinates);
    boost::mpl::for_each<boost::mpl::range_c<int, 0, nb_vars> >(init_ctx);
    
    FillContexts<VarTypesT, ContextsT> fill_ctx(m_vars, contexts);
    
    // Create the global context
    ElementMeshContext<ShapeFunctionT, ContextsT> context(contexts, mapped_coordinates);
    
    const Uint nb_elems = m_elements.connectivity_table().size();
    for(context.element_idx = 0; context.element_idx != nb_elems; ++context.element_idx)
    {
      fill_ctx.elem_idx = context.element_idx;
      boost::mpl::for_each<boost::mpl::range_c<int, 0, nb_vars> >(fill_ctx);
      boost::proto::eval(m_expr, context);
    }
    
  }
private:
  const ExpressionT& m_expr;
  const VarTypesT& m_vars;
  Mesh::CElements& m_elements;
};

////////////////////////////////////////////////////////
// Public interface
///////////////////////////////////////////////////////

/// The actual looping function. Executes expr for each element that matches a shape function in ETypesT.
template<typename ETypesT, typename Expr>
void for_each_element(Common::Component& root, const Expr& expr)
{
  // Number of variables
  typedef typename boost::result_of<ExprVarArity(Expr)>::type nb_vars;
  
  // init empty vector that will store variable indices
  typedef boost::mpl::vector_c<Uint> numbers_empty;
  
  // Fill the vector with indices 0 to 9, so we allow 10 different (field or node related) variables in an expression
  typedef typename boost::mpl::copy<
      boost::mpl::range_c<int,0,nb_vars::value>
    , boost::mpl::back_inserter< numbers_empty >
    >::type range_nb_vars;
  
  // Get the type for each variable that is used, or set to boost::mpl::void_ for unused indices
  typedef typename boost::mpl::transform<range_nb_vars, DefineTypeOp<boost::mpl::_1, Expr > >::type expr_types;
  
  typedef typename boost::fusion::result_of::as_vector<expr_types>::type FusionVarsT;
  
  FusionVarsT vars;
  CopyNumberedVars<FusionVarsT> ctx(vars);
  boost::proto::eval(expr, ctx);
  
  // Evaluate the expression
  BOOST_FOREACH(Mesh::CElements& elements, Mesh::recursive_range_typed<Mesh::CElements>(root))
  {
    boost::mpl::for_each<ETypesT>(ElementLooper<Expr, FusionVarsT, nb_vars::value>(expr, vars, elements));
  }
}

} // namespace Proto
} // namespace Actions
} // namespace CF

#endif // CF_Actions_ProtoElementLooper_hpp