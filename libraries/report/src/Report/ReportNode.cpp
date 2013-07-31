#include "ReportNode.hpp"
#include <typeinfo>

namespace report_generator
{

ReportNode::ReportNode( const ReportIterator nodeIt, const size_t& index, ReportTree* tree )
	: _nodeIt( nodeIt )
	, _parent( NULL )
	, _tree( tree )
	, _index( index )
{
}

ReportNode::ReportNode( const ReportIterator nodeIt, const size_t& index, ReportTree* tree, ReportNode* parent )
	: _nodeIt( nodeIt )
	, _parent( parent )
	, _tree( tree )
	, _index( index )
{
}

ReportNode::~ReportNode()
{
}

ReportNode ReportNode::appendNext( std::shared_ptr< be::Element > element )
{
	std::stringstream sstr;
	size_t index = _index;
	ReportTree node;
	ReportIterator nodeIt = _nodeIt;
	
	if( _parent )
		sstr << _parent->getIndex() << "-";
	sstr << ++index;
	
	node.add( sstr.str(), element );
	
	if( _parent )
	{
		_parent->getSecond()->add_child( kGroup + ".", node );
	}
	else
	{
		_tree->add_child( kReport + ".", node );
	}
	return ReportNode( ++nodeIt, index, _tree, _parent );
}

ReportNode ReportNode::appendChild( std::shared_ptr< be::Element > element )
{
	std::stringstream sstr;
	sstr << getSecond()->begin()->first << "-" << 0;
	
	ReportTree node;
	node.add( sstr.str(), element );

	getSecond()->add_child( kGroup + ".", node );
	return ReportNode( getSecond()->get_child( kGroup ).begin(), 0, _tree, this );
}

ReportNode* ReportNode::parent()
{
	try
	{
		if( _parent == NULL )
			throw std::runtime_error( "parent: This node has no parent." );
		return _parent;
	}
	catch( std::runtime_error& e )
	{
		LOG_ERROR( e.what() );
		throw;
	}
}

ReportTree* ReportNode::getSecond()
{
	return const_cast< ReportTree* >( &_nodeIt->second );
}


size_t ReportNode::getIndex()
{
	return _index;
}

}

