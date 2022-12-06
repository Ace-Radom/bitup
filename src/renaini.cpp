#include"renaini.h"

// blank chars' set
const char rena::blank_chars[] = " \t\n\r\f\v";

/**
 * @brief delete all given chars at the beginning and the end of a string
 * 
 * @param __str the string needs to be processed
 * @param __erase_chars given chars ( default: blank_chars[] in @a namespace @c rena )
 */
std::string& rena::erase_blank_chars( std::string& __str , const char* __erase_chars = blank_chars ){
    __str.erase( __str.find_last_not_of( __erase_chars ) + 1 );
    __str.erase( 0 , __str.find_first_not_of( __erase_chars ) );
    return __str;
}

/**
 * @brief delete all given chars at the beginning of a string
 * 
 * @param __str the string needs to be processed
 * @param __erase_chars given chars ( default: blank_chars[] in @a namespace @c rena )
 */
std::string& rena::erase_blank_chars_at_begin( std::string& __str , const char* __erase_chars = blank_chars ){
    __str.erase( 0 , __str.find_first_not_of( __erase_chars ) );
    return __str;
}

/**
 * @brief delete all given chars at the end of a string
 * 
 * @param __str the string needs to be processed
 * @param __erase_chars given chars ( default: blank_chars[] in @a namespace @c rena )
 */
std::string& rena::erase_blank_chars_at_end( std::string& __str , const char* __erase_chars = blank_chars ){
    __str.erase( __str.find_last_not_of( __erase_chars ) + 1 );
    return __str;
}

/**
 * @brief judge if this string has only one line
 * 
 * @param __str the string needs to be judged
 */
bool rena::is_single_line_str( const std::string& __str ){
    return ( __str.find_first_of( "\r\n" ) == std::string::npos );
    // if \r and \n aren't found, function find_first_of will return npos
}

/**
 * @brief compare two strings but ignore caps differences
 * 
 * @param __lstr left string
 * @param __rstr right string
 * 
 * @return int. 
 *         When __lstr >  __rstr, return >= 1;
 *         When __lstr == __rstr, return == 0;
 *         When __lstr <  __rstr, return <= 1
 * 
 */
int rena::compare_str_without_caps( const char* __lstr , const char* __rstr ){
    if ( __lstr == __rstr )
    {
        return 0;
    }
    if ( __lstr == NULL )
    {
        return -1;
    }
    if ( __rstr == NULL )
    {
        return 1;
    }
    // Q: Why special judgement isn't needed in the second and third if-statement?
    //    If (I mean if) both __lstr and __rstr contains no char (or, same as NULL), does that still operate right?
    // A: Sure. When they are both NULL, 0 has already been returned in the first if-statement

    int l = 0;
    int r = 0;

    do // start compare
    {
        if ( ( ( l = ( *( __lstr++ ) ) ) >= 'A' ) && ( l <= 'Z' ) )
        {
            l -= ( 'A' - 'a' );
        }
        if ( ( ( r = ( *( __rstr++ ) ) ) >= 'A' ) && ( r <= 'Z' ) )
        {
            r -= ( 'A' - 'a' );
        }
    } while ( l && ( l == r ) );

    return ( l - r );
}

bool rena::cswc_t::operator()( const std::string& __lstr , const std::string& __rstr ) const {
    return ( compare_str_without_caps( __lstr.c_str() , __rstr.c_str() ) < 0 );
}

/**
 * @brief judge dirty
 */
bool rena::ini_node_t::is_dirty( void ) const {
    if ( m_owner_ptr != NULL )
    {
        return m_owner_ptr -> is_dirty();
    }
    return false;
}

/**
 * @brief set dirty signature
 * 
 * @param __dirty
 */
void rena::ini_node_t::set_dirty( bool __dirty ){
    if ( m_owner_ptr != NULL )
    {
        m_owner_ptr -> set_dirty( __dirty );
    }
}

/**
 * @brief get node type
 */
int rena::ini_node_t::ntype( void ) const {
    return m_ini_ntype;
}

/**
 * @brief get node owner
 */
rena::ini_node_t* rena::ini_node_t::get_owner( void ) const {
    return m_owner_ptr;
}

/**
 * @brief the ostream function for @a class @c ini_node_t
 * 
 * @param __os ostream
 * @param __ini_node @a class @c ini_node_t
 */
std::ostream& rena::operator<<( std::ostream& __os , const ini_node_t& __ini_node ){
    __ini_node >> __os;
    return __os;
}

/**
 * @brief judge if the string which has already been processed by function @c erase_blank_chars conforms to the 
 *        format which has been defined by @a class @c ini_nilline_t
 * @param __str the string
 */
bool rena::ini_nilline_t::is_ntype( const std::string& __str ){
    return __str.empty();
}


/**
 * @brief try to create and pre-set the @c ini_nilline_t object directly with the string
 * 
 * @param __str the string for creating @c ini_nilline_t object
 * @param __owner_ptr node owner
 */
rena::ini_node_t* rena::ini_nilline_t::try_create( const std::string& __str , ini_node_t* __owner_ptr ){
    if ( !is_ntype( __str ) )
    {
        return NULL;
    }
    
    return ( new ini_nilline_t( __owner_ptr ) );
}

/**
 * @brief transform node infos to ostream
 * 
 * @param __os ostream
 */
const rena::ini_node_t& rena::ini_nilline_t::operator>>( std::ostream& __os ) const {
    __os << std::endl;
    return *this;
}

/**
 * @brief judge if the string which has already been processed by function @c erase_blank_chars conforms to the 
 *        format which has been defined by @a class @c ini_comment_t
 * @param __str the string
 */
bool rena::ini_comment_t::is_ntype( const std::string& __str ){
    return ( !__str.empty() && ( ( __str.at( 0 ) == ';' ) || ( __str.at( 0 ) == '#' ) ) );
}

/**
 * @brief try to create and pre-set the @c ini_comment_t object directly with the string
 * 
 * @param __str the string for creating @c ini_comment_t object
 * @param __owner_ptr node owner
 */
rena::ini_node_t* rena::ini_comment_t::try_create( const std::string& __str , ini_node_t* __owner_ptr ){
    if ( !is_ntype( __str ) )
    {
        return NULL;
    }

    ini_comment_t* node_ptr = new ini_comment_t( __owner_ptr );
    node_ptr -> m_str_text = __str;
    return node_ptr;
}

/**
 * @brief transform node infos to ostream
 * 
 * @param __os ostream
 */
const rena::ini_node_t& rena::ini_comment_t::operator>>( std::ostream& __os ) const {
    __os << m_str_text << std::endl;
    return *this;
}

/**
 * @brief judge if the string which has already been processed by function @c erase_blank_chars conforms to the 
 *        format which has been defined by @a class @c ini_keyvalue_t
 * 
 * @param __str the string
 * 
 * @warning no idea why this function only returns flase, and I didn't found where does this function be used either
 */
bool rena::ini_keyvalue_t::is_ntype( const std::string& __str ){
    if ( __str.empty() )
    {
        return false;
    }

    size_t eq_pos = __str.find( '=' );
    // find equal sign position

    if ( ( eq_pos == 0 ) || ( eq_pos == std::string::npos ) ) // equal sign at the first pos. or not found
    {
        return false;
    }
    return false;
}

/**
 * @brief try to create and pre-set the @c ini_keyvalue_t object directly with the string
 * 
 * @param __str the string for creating @c ini_keyvalue_t object
 * @param __owner_ptr node owner
 */
rena::ini_node_t* rena::ini_keyvalue_t::try_create( const std::string& __str , ini_node_t* __owner_ptr ){
    if ( __str.empty() )
    {
        return NULL;
    }

    size_t eq_pos = __str.find( '=' );
    // find equal sign position

    if ( ( eq_pos == 0 ) || ( eq_pos == std::string::npos ) )
    {
        return NULL;
    }

    ini_keyvalue_t* node_ptr = new ini_keyvalue_t( __owner_ptr );

    node_ptr -> m_str_kname = __str.substr( 0 , eq_pos );
    node_ptr -> m_str_value = __str.substr( eq_pos + 1 );

    erase_blank_chars( node_ptr -> m_str_kname );
    erase_blank_chars( node_ptr -> m_str_value );

    return node_ptr;
}

/**
 * @brief transform node infos to ostream
 * 
 * @param __os ostream
 */
const rena::ini_node_t& rena::ini_keyvalue_t::operator>>( std::ostream &__os ) const {
    __os << m_str_kname << '=' << m_str_value << std::endl;
    return *this;
}

#ifdef TEMPLATE_DEFINATION_IN_CXX

/**
 * @brief read int from key value
 */
template <typename __integer_type> __integer_type rena::ini_keyvalue_t::get_ivalue() const {

#if CPP_STANDARD < 201103L

    return static_cast <__integer_type> ( atol( m_str_value.c_str() ) );
    // CPP_STANDARD < 201103L

#else

    return static_cast <__integer_type> ( atoll( m_str_value.c_str() ) );
    // CPP_STANDARD >= 201103L

#endif

}

/**
 * @brief read key value with default NUM setting
 */
template <typename __base_type> __base_type rena::ini_keyvalue_t::get_default( __base_type __default ) const {
    if ( empty() )
    {
        return __default;
    }
    return ( __base_type ) ( *this );
}

/**
 * @brief write integer key value
 * 
 * @param __value the integer value
 */
template <typename __integer_type> rena::ini_keyvalue_t& rena::ini_keyvalue_t::set_ivalue( __integer_type __value ){
    std::ostringstream ostr;
    ostr << __value;
    sl_nbabae_set_value( ostr.str() );
    return *this;
}

/**
 * @brief write float key value
 * 
 * @param __value the float value
 * @param __precision float precision
 */
template <typename __float_type> rena::ini_keyvalue_t& rena::ini_keyvalue_t::set_fvalue( __float_type __value , std::streamsize __precision ){
    std::ostringstream ostr;
    ostr.precision( __precision );
    ostr << __value;
    sl_nbabae_set_value( ostr.str() );
    return *this;
}

/**
 * @brief when value = NULL, try to update it to given value, and then return key value
 */
template <typename __base_type> __base_type rena::ini_keyvalue_t::try_set( __base_type __value ){
    if ( empty() )
    {
        this -> operator= ( __value );
    }

    return ( __base_type ) ( *this );
}

#endif

#ifdef OPERATOR_BOOL_DEFINATION_IN_CXX

/**
 * @brief bool read operator
 */
rena::ini_keyvalue_t::operator bool () const {
    if ( compare_str_without_caps( m_str_value.c_str() , "true" ) == 0 )
    {
        return true;
    }
    if ( compare_str_without_caps( m_str_value.c_str() , "false" ) == 0 )
    {
        return false;
    }
    return ( this -> operator int() != 0 );
}

#endif

/**
 * @brief get key name
 */
const std::string& rena::ini_keyvalue_t::key( void ) const {
    return m_str_kname;
}

/**
 * @brief get key value
 */
const std::string& rena::ini_keyvalue_t::value( void ) const {
    return m_str_value;
}

#ifdef EMPTY_FUNCTION_DEFINATION_IN_CXX

/**
 * @brief judge if key value is empty
 */
bool rena::ini_keyvalue_t::empty( void ) const {
    return m_str_value.empty();
}

#endif

/**
 * @brief set key value
 * 
 * @param __value key value to be set
 */
void rena::ini_keyvalue_t::set_value( const std::string& __value ){
    std::string str = __value.substr( 0 , __value.find_first_of( "\r\n" ) );
    sl_nbabae_set_value( erase_blank_chars( str ) );
    return;
}

/**
 * @brief set single-line and has no blank chars at the beginning and the end's data as key value
 * 
 * @param __str_single_line the single-line value
 */
void rena::ini_keyvalue_t::sl_nbabae_set_value( const std::string& __str_single_line ){
    if ( __str_single_line != m_str_value )
    {
        m_str_value = __str_single_line;
        set_dirty( true );
    }
    return;
}

/**
 * @brief judge if the string which has already been processed by function @c erase_blank_chars conforms to the 
 *        format which has been defined by @a class @c ini_section_t
 * 
 * @param __str the string
 */
bool rena::ini_section_t::is_ntype( const std::string& __str ){
    return ( !__str.empty() && ( __str.at( 0 ) == '[' ) && ( __str.at( __str.size() - 1 ) == ']' ) );
}

/**
 * @brief try to create and pre-set the @c ini_section_t object directly with the string
 * 
 * @param __str the string for creating @c ini_section_t object
 * @param __owner_ptr node owner
 */
rena::ini_node_t* rena::ini_section_t::try_create( const std::string& __str , ini_node_t* __owner_ptr ){
    if ( !is_ntype( __str ) )
    {
        return NULL;
    }

    ini_section_t* node_ptr = new ini_section_t( __owner_ptr );
    node_ptr -> m_str_name = __str;

    erase_blank_chars_at_end( node_ptr -> m_str_name , "]" );
    erase_blank_chars_at_begin( node_ptr -> m_str_name , "[" );
    erase_blank_chars( node_ptr -> m_str_name );

    // 将 自身 作为 节点 加入到 m_xlst_node 中，但并不意味着 m_xlst_node 
    // 的 首个节点 就一定是 自身节点，因为 xini_file_t 在加载过程中，
    // 会调用 pop_tail_comment() 操作，这有可能在 m_xlst_node 前端新增
    // 一些 注释/空行节点。所以在进行 流输出 操作时，自身节点 则可起到 占位行
    // 的作用，详细过程可参看 operator >> 的实现流程
    node_ptr -> m_lst_node.push_back( node_ptr );

    return node_ptr;
}

rena::ini_section_t::~ini_section_t( void ){
    for ( std::list <ini_node_t*>::iterator itlst = m_lst_node.begin() ; itlst != m_lst_node.end() ; ++itlst )
    {
        if ( ( *itlst ) -> ntype() != SECTION )
        {
            delete ( *itlst );
        }
    }

    m_lst_node.clear();
    m_map_ndkv.clear();
}

/**
 * @brief transform node info to ostream
 * 
 * @param __os ostream
 */
const rena::ini_node_t& rena::ini_section_t::operator>>( std::ostream& __os ) const {
    for ( std::list <ini_node_t*>::const_iterator itlst = m_lst_node.begin() ; itlst != m_lst_node.end() ; ++itlst )
    {
        if ( static_cast <ini_section_t*>( const_cast <ini_node_t*>( *itlst ) ) == this )
        {
            if ( !m_str_name.empty() )
            {
                __os << "[" << m_str_name << "]" << std::endl;
            }
        }
        else
        {
            **itlst >> __os;
        }
    }

    return *this;
}

/**
 * @brief key -> value index operator
 * 
 * @param __str_key the key
 */
rena::ini_keyvalue_t& rena::ini_section_t::operator[]( const std::string& __str_key ){
    assert( is_single_line_str( __str_key ) );

    std::string str_nkey = __str_key;
    erase_blank_chars( str_nkey );

    ini_keyvalue_t* knode_ptr = find_knode( str_nkey );
    if ( knode_ptr != NULL )
    {
        return *knode_ptr;
    }

    // 若索引的键值节点并未在节点表中，
    // 则新增此键值节点，但并不设置脏标识，
    // 避免存储不必要的空键值节点

    knode_ptr = static_cast <ini_keyvalue_t*>( ini_keyvalue_t::try_create( str_nkey + "=" , get_owner() ) );
    assert( knode_ptr != NULL );

    m_lst_node.push_back( knode_ptr );
    m_map_ndkv.insert( std::make_pair( str_nkey , knode_ptr ) );

    return *knode_ptr;
}

/**
 * @brief get section name
 */
const std::string& rena::ini_section_t::name( void ) const {
    return m_str_name;
}

/**
 * @brief get section size (how many nodes does this section contain)
 */
size_t rena::ini_section_t::size( void ) const {
    return m_lst_node.size();
}

/**
 * @brief judge if this section is empty
 */
bool rena::ini_section_t::empty() const {
    return m_lst_node.empty();
}

bool rena::ini_section_t::has_end_nilline( void ){
    if ( !m_lst_node.empty() && ( m_lst_node.back() -> ntype() == NILLINE ) )
    {
        return true;
    }
    return false;
}

/**
 * @brief get the iterator of the first key-value node in the section's node list
 */
rena::ini_section_t::iterator rena::ini_section_t::begin_kv( void ){
    iterator iter = m_lst_node.begin();
    if ( ( *iter ) -> ntype() == KEYVALUE )
    {
        return iter;
    }
    return next_kv( iter );
}

/**
 * @brief get the iterator of the first key-value node in the section's node list
 */
rena::ini_section_t::const_iterator rena::ini_section_t::begin_kv( void ) const {
    const_iterator iter = m_lst_node.begin();
    if ( ( *iter ) -> ntype() == KEYVALUE )
    {
        return iter;
    }
    return next_kv( iter );
}

/**
 * @brief get the iterator of the next key-value node in the section's node list
 * 
 * @param __iter given iterator
 */
rena::ini_section_t::iterator rena::ini_section_t::next_kv( iterator __iter ){
    const iterator iter_end = m_lst_node.end();
    if ( __iter != iter_end )
    {
        while ( ++__iter != iter_end )
        {
            if ( ( *__iter ) -> ntype() == KEYVALUE )
            {
                return __iter;
            }
        }
    }

    return iter_end;
}

/**
 * @brief get the iterator of the next key-value node in the section's node list
 * 
 * @param __iter given iterator
 */
rena::ini_section_t::const_iterator rena::ini_section_t::next_kv( const_iterator __iter ) const {
    const const_iterator iter_end = m_lst_node.end();
    if ( __iter != iter_end )
    {
        while ( ++__iter != iter_end )
        {
            if ( ( *__iter ) -> ntype() == KEYVALUE )
            {
                return __iter;
            }
        }
    }

    return iter_end;
}

/**
 * @brief add nilline / commend / key-value node
 * 
 * @param __node_ptr nilline / commend / key-value node
 * 
 * @return bool: success as true, failed as false
 */
bool rena::ini_section_t::push_node( ini_node_t* __node_ptr ){
    if ( __node_ptr == NULL )
    {
        return false;
    }

    if ( ( __node_ptr -> ntype() == NILLINE ) || ( __node_ptr -> ntype() == COMMENT ) )
    {
        m_lst_node.push_back( __node_ptr );
        return true;
    }

    if ( __node_ptr -> ntype() == KEYVALUE )
    {
        ini_keyvalue_t* node_kvptr = static_cast <ini_keyvalue_t*>( __node_ptr );
        if ( find_knode( node_kvptr -> key() ) != NULL )
        {
            return false;
        }

        m_lst_node.push_back( __node_ptr );
        m_map_ndkv.insert( std::make_pair( node_kvptr -> key() , node_kvptr ) );
        return true;
    }

    return false;
}

/**
 * @brief find key-value node in section
 * 
 * @param __str_key node-index str
 * 
 * @return ini_keyvalue_t*: success returning the corresponding node, failed returnning NULL
 */
rena::ini_keyvalue_t* rena::ini_section_t::find_knode( const std::string& __str_key ) const {
    map_ndkv_t::const_iterator itfind = m_map_ndkv.find( __str_key );
    if ( itfind != m_map_ndkv.end() )
    {
        return itfind -> second;
    }
    return NULL;
}

/**
 * @brief Remove the comment node under the non-current section from the end of the node list (delimited by a nilline node)
 * 
 * @param __lst_comm comment node list
 * @param __bt_front indicates that the operation is a node returned from before/after the xlst_comm append
 * 
 * @return size_t: how many nodes have been poped
 */
size_t rena::ini_section_t::pop_tail_comment( std::list <ini_node_t*>& __lst_comm , bool __bt_front ){
    std::list <ini_node_t*> lst_node;

    size_t st_line = 0;
    size_t st_maxl = m_lst_node.size();

    // node list only contains three types of nodes: key-value, nilline, comment
    // and also (plus itself) section node

    while ( ( st_line++ < st_maxl ) && !m_lst_node.empty() )
    {
        ini_node_t* node_ptr = m_lst_node.back();

        // find nilline
        if ( node_ptr -> ntype() == NILLINE )
        {
            if ( st_line > 1 )
            {
                break;
            }

            // only the first can be nilline
            lst_node.push_front( node_ptr );
            m_lst_node.pop_back();
            continue;
        }

        if ( ( node_ptr -> ntype() == KEYVALUE ) || ( node_ptr -> ntype() == SECTION ) )
        {
            m_lst_node.splice( m_lst_node.end() , lst_node );
            break;
        }

        if ( node_ptr -> ntype() == COMMENT )
        {
            lst_node.push_front( node_ptr );
            m_lst_node.pop_back();
        }
        else
        {
            assert( false );
            // unrecognised node types
        }
    }

    size_t st_count = lst_node.size();
    if ( st_count > 0 )
    {
        if ( __bt_front )
        {
            lst_node.splice( lst_node.end() , __lst_comm );
            __lst_comm.swap( lst_node );
        }
        else 
        {
            __lst_comm.splice( __lst_comm.end() , lst_node );
        }
    }

    return st_count;
}

rena::ini_node_t* rena::ini_file::make_node( const std::string& __str_line , ini_file* __owner_ptr ){
    ini_node_t* node_ptr = NULL;

#define TRY_CREATE( nptr , node , owner )                       \
        do                                                      \
        {                                                       \
            nptr = node::try_create( __str_line , owner );      \
            if ( nptr != NULL )                                 \
            {                                                   \
                return nptr;                                    \
            }                                                   \
        } while ( 0 )

    TRY_CREATE( node_ptr , ini_nilline_t  , __owner_ptr );
    TRY_CREATE( node_ptr , ini_comment_t  , __owner_ptr );
    TRY_CREATE( node_ptr , ini_section_t  , __owner_ptr );
    TRY_CREATE( node_ptr , ini_keyvalue_t , __owner_ptr );

#undef TRY_CREATE

    return node_ptr;
}

/**
 * @brief transform node info to ostream
 * 
 * @param __os ostream
 */
const rena::ini_node_t& rena::ini_file::operator>>( std::ostream& __os ) const {
    for ( std::list <ini_section_t*>::const_iterator itlst = m_lst_sect.begin() ; itlst != m_lst_sect.end() ; ++itlst )
    {
        if ( ( *itlst ) -> empty() )
        {
            continue;
        }
        **itlst >> __os;
        if ( !( *itlst ) -> has_end_nilline() && ( ( *itlst ) != m_lst_sect.back() ) )
        {
            __os << std::endl;
        }
    }

    return *this;
}

/**
 * @brief get dirty signature
 */
bool rena::ini_file::is_dirty( void ) const {
    return m_bt_dirty;
}

/**
 * @brief set dirty signature
 * 
 * @param __dirty dirty signature
 */
void rena::ini_file::set_dirty( bool __dirty ){
    m_bt_dirty = __dirty;
    return;
}

rena::ini_file& rena::ini_file::operator<<( std::istream& __is ){
    ini_section_t* sect_ptr = NULL;
    // the section in operation

    if ( m_lst_sect.empty() ) // section empty, create one new section node
    {
        sect_ptr = new ini_section_t( this );
        m_lst_sect.push_back( sect_ptr );

        assert( m_map_sect.empty() );
        m_map_sect.insert( std::make_pair( std::string( "" ) , sect_ptr ) );
    }
    else // get the last section as this section node in operation
    {
        sect_ptr = m_lst_sect.back();

        if ( !sect_ptr -> has_end_nilline() )
        {
            sect_ptr -> push_node( new ini_nilline_t( this ) );
        }
    }

    // start ini node list creation
    while ( !__is.eof() )
    {
        std::string str_line;
        std::getline( __is , str_line );
        erase_blank_chars( str_line );
        // read ini text

        if ( __is.eof() && str_line.empty() )
        {
            break;
        }
        // last nilline won't be added to node list, in order to provide from continue adding nilline at the end

        ini_node_t* node_ptr = make_node( str_line , this );
        // new node

        if ( node_ptr == NULL )
        {
            continue;
        }

        if ( node_ptr -> ntype() == SECTION )
        {
            sect_ptr = push_sect( static_cast <ini_section_t*>( node_ptr ) , sect_ptr );
            if ( sect_ptr != static_cast <ini_section_t*>( node_ptr ) )
            {
                delete node_ptr; // add new section failed, delete it
            }
            else
            {
                set_dirty( true ); // add new section success, set dirty signature
            }
            continue;
        }

        if ( sect_ptr -> push_node( node_ptr ) ) // add current section
        {
            set_dirty( true );
        }
        else // add current section failed
        {
            delete node_ptr;
            // 可能的失败原因为：其为键值节点，与分节节点表中已有的节点索引键冲突
        }
    }

    return *this;    
}

/**
 * @brief section index operator
 */
rena::ini_section_t& rena::ini_file::operator[]( const std::string& __str_sect ){
    assert( is_single_line_str( __str_sect ) );

    std::string str_name = __str_sect;
    erase_blank_chars( str_name );
    erase_blank_chars_at_end( str_name , "]" );
    erase_blank_chars_at_begin( str_name , "[" );
    erase_blank_chars( str_name );

    ini_section_t* sect_ptr = find_sect( str_name );
    if ( sect_ptr != NULL )
    {
        return *sect_ptr;
    }

    // 若索引的分节并未在分节的节点表中，
    // 则新增此分节，但并不设置脏标识，
    // 避免存储不必要的空分节

    sect_ptr = static_cast <ini_section_t*>( ini_section_t::try_create( "[" + str_name + "]" , this ) );
    assert( sect_ptr != NULL );

    m_lst_sect.push_back( sect_ptr );
    m_map_sect.insert( std::make_pair( str_name , sect_ptr ) );

    return *sect_ptr;
}

/**
 * @brief load ini datas from given file
 * 
 * @param __str_filepath file path
 * 
 * @return bool: success as true, failed as false
 * 
 * @note 
 * * load() 操作的成功与否，并不影响后续的键值读写操作，
 * * 其只能标示 xini_file_t 对象是否关联可至指定路径
 * * （本地磁盘或远程网络等的）文件。
 */
bool rena::ini_file::load( const std::string& __str_filepath ){
    release();
    // release this object first

    m_str_path = __str_filepath;

    if ( __str_filepath.empty() )
    {
        return false;
    }

    std::ifstream rFile( __str_filepath.c_str() );
    // open ini file

    if ( !rFile.is_open() )
    {
        return false;
    }

    // Skip encoding information of the character stream at stream begin
    while ( !rFile.eof() )
    {
        int rc = rFile.get(); // read char
        if ( std::iscntrl( rc ) || std::isprint( rc ) )
        {
            rFile.putback( static_cast <char>( rc ) );
            break;
        }

        m_str_head.push_back( static_cast <char>( rc ) );
    }

    *this << rFile;
    set_dirty( false );

    return true;
}

/**
 * @brief release object
 */
void rena::ini_file::release( void ){
    if ( is_dirty() )
    {
        dump( m_str_path );
        set_dirty( false );
    }
    m_str_path.clear();
    m_str_head.clear();

    for ( std::list <ini_section_t*>::iterator itlst = m_lst_sect.begin() ; itlst != m_lst_sect.end() ; ++itlst )
    {
        delete *itlst;
    }

    m_lst_sect.clear();
    m_map_sect.clear();

    return;
}

/**
 * @brief get file path
 */
const std::string& rena::ini_file::filepath( void ) const {
    return m_str_path;
}

/**
 * @brief get section NUM
 */
size_t rena::ini_file::sect_count( void ) const {
    return m_lst_sect.size();
}

bool rena::ini_file::dump( const std::string& __str_filepath ){
    std::ofstream wFile( __str_filepath.c_str() , std::ios_base::trunc );
    // open file 

    if ( !wFile.is_open() )
    {
        return false;
    }

    if ( !m_str_head.empty() )
    {
        wFile << m_str_head.c_str();
    }
    *this >> wFile;

    return true;
}

/**
 * @brief search for section
 * 
 * @param __str_sect section
 */
rena::ini_section_t* rena::ini_file::find_sect( const std::string& __str_sect ) const {
    map_section_t::const_iterator itfind = m_map_sect.find( __str_sect );
    if ( itfind != m_map_sect.end() )
    {
        return itfind -> second;
    }
    return NULL;
}

rena::ini_section_t* rena::ini_file::push_sect( ini_section_t* __new_ptr , ini_section_t* __sect_ptr ){
    ini_section_t* find_ptr = find_sect( __new_ptr -> name() );
    // find same-name section

    if ( find_ptr == NULL ) // same-name section not found, write new section to end of node list
    {
        m_lst_sect.push_back( __new_ptr );
        m_map_sect.insert( std::make_pair( __new_ptr -> name() , __new_ptr ) );

        
        __sect_ptr -> pop_tail_comment( __new_ptr -> m_lst_node , true );
        // mova all comment nodes at the end of the node list in operation before 
        // to in the front of new-added section node

        __sect_ptr = __new_ptr;
        // set new section node as the current section node and return
    }
    else if ( find_ptr != __sect_ptr )
    {
        // move all comment nodes at the end of the section node in operation
        // to after the same-name section's node list

        if ( !find_ptr -> has_end_nilline() )
        {
            find_ptr -> push_node( new ini_nilline_t( this ) );
        }

        __sect_ptr -> pop_tail_comment( find_ptr -> m_lst_node , false );
        // add comment node

        if ( !find_ptr -> has_end_nilline() )
        {
            find_ptr -> push_node( new ini_nilline_t( this ) );
        }

        __sect_ptr = find_ptr;
        // set the same-name section as the current section node and return
    }
    
    return __sect_ptr;
}

/**
 * @brief stream readin operator for @a class @c ini_file
 * 
 * @param __is istream
 * @param __ini_file ini_file
 */
std::istream& rena::operator>>( std::istream& __is , ini_file& __ini_file ){
    __ini_file << __is;
    return __is;
}