// Copyright (C) 2010 von Karman Institute for Fluid Dynamics, Belgium
//
// This software is distributed under the terms of the
// GNU Lesser General Public License version 3 (LGPLv3).
// See doc/lgpl.txt and doc/gpl.txt for the license text.

#include "Common/BasicExceptions.hpp"
#include "Common/CBuilder.hpp"

#include "LibSF.hpp"
#include "Line2DLagrangeP3.hpp"
#include "Point1DLagrangeP1.hpp"

namespace CF {
namespace Mesh {
namespace SF {

////////////////////////////////////////////////////////////////////////////////

Common::ComponentBuilder < Line2DLagrangeP3, ElementType, LibSF > aLine2DLagrangeP3_Builder;

////////////////////////////////////////////////////////////////////////////////

Line2DLagrangeP3::Line2DLagrangeP3(const std::string& name) : Line2D(name)
{
  m_nb_nodes = nb_nodes;
  m_order = order;
}

////////////////////////////////////////////////////////////////////////////////

std::string Line2DLagrangeP3::element_type_name() const
{
  return type_name();
}

////////////////////////////////////////////////////////////////////////////////

Real Line2DLagrangeP3::compute_volume(const NodesT& coord) const
{
  return 0;
}

////////////////////////////////////////////////////////////////////////////////

Real Line2DLagrangeP3::compute_area(const NodesT& coord) const
{
  return area(coord);
}

////////////////////////////////////////////////////////////////////////////////

bool Line2DLagrangeP3::is_coord_in_element(const RealVector& coord, const NodesT& nodes) const
{
	return false;
}

////////////////////////////////////////////////////////////////////////////////

const CF::Mesh::ElementType::FaceConnectivity& Line2DLagrangeP3::face_connectivity() const
{
  static FaceConnectivity connectivity;
  if(connectivity.face_first_nodes.empty())
  {
    connectivity.face_first_nodes = boost::assign::list_of(0);
    connectivity.face_node_counts.assign(1, 3); //1 row, 3 columns - indexes 0,1,2 of the nodes of the line
    connectivity.face_nodes = boost::assign::list_of(0)(1)(2);
  }
  return connectivity;
}

////////////////////////////////////////////////////////////////////////////////

const CF::Mesh::ElementType& Line2DLagrangeP3::face_type(const CF::Uint face) const
{
  const static Point1DLagrangeP1 facetype;
  return facetype;
}

////////////////////////////////////////////////////////////////////////////////

void Line2DLagrangeP3::shape_function(const MappedCoordsT& mappedCoord, ShapeFunctionsT& shapeFunc)
{
  throw Common::NotImplemented( FromHere(), "" );

  shapeFunc[0] = 0.5 * (mappedCoord[KSI]*mappedCoord[KSI] - mappedCoord[KSI]);
  shapeFunc[1] = 0.5 * (mappedCoord[KSI]*mappedCoord[KSI] + mappedCoord[KSI]);
  shapeFunc[2] = (1.0 - mappedCoord[KSI]*mappedCoord[KSI]);
}

////////////////////////////////////////////////////////////////////////////////

void Line2DLagrangeP3::mapped_gradient(const MappedCoordsT& mappedCoord, MappedGradientT& result)
{
  throw Common::NotImplemented( FromHere(), "" );

  result(XX, 0) = mappedCoord[KSI]-0.5;
  result(XX, 1) = mappedCoord[KSI]+0.5;
  result(XX, 2) = -2.0*mappedCoord[KSI]; 
}

////////////////////////////////////////////////////////////////////////////////

void Line2DLagrangeP3::jacobian(const MappedCoordsT& mappedCoord, const NodeMatrixT& nodes, JacobianT& result)
{
  throw Common::NotImplemented( FromHere(), "" );

  result(KSI,XX) = 0.5*(nodes(1, XX) - nodes(0, XX));
  result(KSI,YY) = 0.5*(nodes(1, YY) - nodes(0, YY));
}

////////////////////////////////////////////////////////////////////////////////

void Line2DLagrangeP3::normal(const MappedCoordsT& mappedCoord, const NodeMatrixT& nodes, CoordsT& result)
{
  throw Common::NotImplemented( FromHere(), "" );

  result[XX] = 0.5*( nodes(1, YY) - nodes(0, YY));
  result[YY] = 0.5*(-nodes(1, XX) + nodes(0, XX));
}

////////////////////////////////////////////////////////////////////////////////

Real Line2DLagrangeP3::volume(const NodeMatrixT& nodes)
{
  throw Common::NotImplemented( FromHere(), "" );

  return 0.;
}

////////////////////////////////////////////////////////////////////////////////

Real Line2DLagrangeP3::area(const NodeMatrixT& nodes)
{
  throw Common::NotImplemented( FromHere(), "" );

  return (nodes.row(1)-nodes.row(0)).norm();
}

////////////////////////////////////////////////////////////////////////////////

} // SF
} // Mesh
} // CF