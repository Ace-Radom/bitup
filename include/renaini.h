#ifndef _RENAINI_H_
#define _RENAINI_H_

#include<string>
#include<list>
#include<map>
#include<stdlib.h>
#include<sstream>
#include<fstream>
#include<cassert>

#if defined( __clang__ ) || defined( __GNUC__ ) // is using clang or gcc compiler

    #define CPP_STANDARD __cplusplus

#elif defined( _MSC_VER ) // is using msvc compiler

    #define CPP_STANDARD _MSVC_LANG

#endif
// get C++ standard in use

namespace rena{

    extern const char blank_chars[];

    static inline std::string& erase_blank_chars( std::string& , const char* );
    static inline std::string& erase_blank_chars_at_begin( std::string& , const char* );
    static inline std::string& erase_blank_chars_at_end( std::string& , const char* );

    static inline bool is_single_line_str( const std::string& );
    static int compare_str_without_caps( const char* , const char* );

    /**
     * @struct compare_str_without_caps_t
     * @brief as functor
     */
    typedef struct {
        typedef std::string first_arg_type;
        typedef std::string second_arg_type;
        typedef bool        result_type;

        bool operator()( const std::string& , const std::string& ) const;
    } cswc_t;

    /**
     * @enum ini_node_type_t
     * @brief node type in ini file
     */
    typedef enum {
        UNDEFINE = 0xFFFFFFFF, // 未定义
        FILEROOT = 0x00000000, // 文件根
        NILLINE  = 0x00000100, // 空行
        COMMENT  = 0x00000200, // 注释
        SECTION  = 0x00000300, // 分节
        KEYVALUE = 0x00000400  // 键值
    } ini_ntype_t;

    class ini_keyvalue_t;
    class ini_section_t;
    class ini_comment_t;
    class ini_nilline_t;
    class ini_file;

    /**
     * @class ini_node_t
     * @brief basic ini node infos class
     */
    class ini_node_t{

        friend class ini_file;
        friend class ini_section_t;

        // constructor / destructor
        protected:
            ini_node_t( int ini_ntype , ini_node_t* owner_ptr )
                : m_ini_ntype( ini_ntype )
                , m_owner_ptr( owner_ptr ) {}
            
            virtual ~ini_node_t( void ) {}

        // extensible interface
        public:
            /**
             * stream node infos to ostream
             */
            virtual const ini_node_t& operator>>( std::ostream& __ostr ) const = 0;

            virtual bool is_dirty( void ) const;
            virtual void set_dirty( bool );

        // public interfaces
        public:
            inline int ntype( void ) const;
            inline ini_node_t* get_owner( void ) const;

        // data members
        protected:
            int         m_ini_ntype; // node type
            ini_node_t* m_owner_ptr; // node owner

    }; // class ini_node_t

    inline std::ostream& operator<<( std::ostream& , const ini_node_t& );

    /**
     * @class ini_nilline_t
     * @brief empty line node (in ini file) class
     */
    class ini_nilline_t: public ini_node_t {

        friend class ini_file;

        // common invoking
        protected:
            static bool is_ntype( const std::string& );
            static ini_node_t* try_create( const std::string& , ini_node_t* owner_ptr );

        // construcor/destructor
        protected:
            ini_nilline_t( ini_node_t* owner_ptr )
                : ini_node_t( NILLINE , owner_ptr ) {}

            virtual ~ini_nilline_t( void ) {}

        // overrides
        public:
            virtual const ini_node_t& operator>>( std::ostream& ) const;

    }; // class ini_nilline_t

    /**
     * @class ini_comment_t
     * @brief comment node (in ini file) class
     */
    class ini_comment_t: public ini_node_t {

        friend class ini_file;

        // common invoking
        protected:
            static bool is_ntype( const std::string& );
            static ini_node_t* try_create( const std::string& , ini_node_t* );

        // construcor / destructor
        protected:
            ini_comment_t( ini_node_t* owner_ptr )
                : ini_node_t( COMMENT , owner_ptr ) {}

            virtual ~ini_comment_t( void ) {}

        // overrides
        public:
            virtual const ini_node_t& operator>>( std::ostream& ) const;

        // data member
        protected:
            std::string m_str_text;

    }; // class ini_comment_t

    /**
     * @class ini_keyvalue_t
     * @brief key value node (in ini file) class
     */
    class ini_keyvalue_t: public ini_node_t {
        friend class ini_file;
        friend class ini_section_t;

        // common invoking
        protected:
            static bool is_ntype( const std::string& );
            static ini_node_t* try_create( const std::string& , ini_node_t* );

        // construcor / destructor
        protected:
            ini_keyvalue_t( ini_node_t* owner_ptr )
                : ini_node_t( KEYVALUE , owner_ptr ) {}
            
            virtual ~ini_keyvalue_t( void ) {}

        // overrides
        public:
            virtual const ini_node_t& operator>>( std::ostream& ) const;

        // template<> functions, for operators
        protected:
            /**
             * @brief read int from key value
             */
            template <typename __integer_type> __integer_type get_ivalue() const {
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
            template <typename __base_type> __base_type get_default( __base_type __default ) const {
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
            template <typename __integer_type> ini_keyvalue_t& set_ivalue( __integer_type __value ){
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
            template <typename __float_type> ini_keyvalue_t& set_fvalue( __float_type __value , std::streamsize __precision ){
                std::ostringstream ostr;
                ostr.precision( __precision );
                ostr << __value;
                sl_nbabae_set_value( ostr.str() );
                return *this;
            }

            /**
             * @brief when value = NULL, try to update it to given value, and then return key value
             */
            template <typename __base_type> __base_type try_set( __base_type __value ){
                if ( empty() )
                {
                    this -> operator= ( __value );
                }

                return ( __base_type ) ( *this );
            }

        // operators
        public:

#pragma region basic_data_type_read_operator
               // 基础数据类型的读取运算符

            operator const std::string& () const { return m_str_value;         }
            operator const char*        () const { return m_str_value.c_str(); }

            /**
             * @brief bool read operator
             */
            inline operator bool        () const {
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
            
            operator short              () const { return get_ivalue <short             >(); }
            operator unsigned short     () const { return get_ivalue <unsigned short    >(); }
            operator int                () const { return get_ivalue <int               >(); }
            operator unsigned int       () const { return get_ivalue <unsigned int      >(); }
            operator long               () const { return get_ivalue <long              >(); }
            operator unsigned long      () const { return get_ivalue <unsigned long     >(); }
            operator long long          () const { return get_ivalue <long long         >(); }
            operator unsigned long long () const { return get_ivalue <unsigned long long>(); }

            operator float              () const { return static_cast <float>( this -> operator double() ); }
            operator double             () const { return atof( m_str_value.c_str() ); }

#pragma endregion basic_data_type_read_operator

#pragma region read_value_with_default-set_operator
               // 读取带有默认值的值的()运算符
            
            const std::string& operator()( const std::string& __default ) const { return get_default <const std::string&>( __default ); }
            const char*        operator()( const char*        __default ) const { return get_default <const char*       >( __default ); }
            bool               operator()( bool               __default ) const { return get_default <bool              >( __default ); }
            short              operator()( short              __default ) const { return get_default <short             >( __default ); }
            unsigned short     operator()( unsigned short     __default ) const { return get_default <unsigned short    >( __default ); }
            int                operator()( int                __default ) const { return get_default <int               >( __default ); }
            unsigned int       operator()( unsigned int       __default ) const { return get_default <unsigned int      >( __default ); }
            long               operator()( long               __default ) const { return get_default <long              >( __default ); }
            unsigned long      operator()( unsigned long      __default ) const { return get_default <unsigned long     >( __default ); }
            long long          operator()( long long          __default ) const { return get_default <long long         >( __default ); }
            unsigned long long operator()( unsigned long long __default ) const { return get_default <unsigned long long>( __default ); }
            float              operator()( float              __default ) const { return get_default <float             >( __default ); }
            double             operator()( double             __default ) const { return get_default <double            >( __default ); }

#pragma endregion read_value_with_default-set_operator

#pragma region read_value_with_default-set_and_try_to_update_NULL_key
               // 与上一重载的()运算符功能类似 但会尝试将空键值更新为给到的默认值

            const std::string& try_value( const std::string& __default) { return try_set <const std::string&>( __default ); }
            const char*        try_value( const char*        __default) { return try_set <const char*       >( __default ); }
            bool               try_value( bool               __default) { return try_set <bool              >( __default ); }
            short              try_value( short              __default) { return try_set <short             >( __default ); }
            unsigned short     try_value( unsigned short     __default) { return try_set <unsigned short    >( __default ); }
            int                try_value( int                __default) { return try_set <int               >( __default ); }
            unsigned int       try_value( unsigned int       __default) { return try_set <unsigned int      >( __default ); }
            long               try_value( long               __default) { return try_set <long              >( __default ); }
            unsigned long      try_value( unsigned long      __default) { return try_set <unsigned long     >( __default ); }
            long long          try_value( long long          __default) { return try_set <long long         >( __default ); }
            unsigned long long try_value( unsigned long long __default) { return try_set <unsigned long long>( __default ); }
            float              try_value( float              __default) { return try_set <float             >( __default ); }
            double             try_value( double             __default) { return try_set <double            >( __default ); }

#pragma endregion read_value_with_default-set_and_try_to_update_NULL_key

#pragma region basic_data_type_write_operator
               // 基础数据类型的写入运算符
            
            ini_keyvalue_t& operator=( const std::string& __value ) { set_value( __value ); return *this;                              }
            ini_keyvalue_t& operator=( const char*        __value ) { set_value( __value ); return *this;                              }
            ini_keyvalue_t& operator=( bool               __value ) { sl_nbabae_set_value( __value ? "true" : "false" ); return *this; }
            ini_keyvalue_t& operator=( short              __value ) { return set_ivalue<short             >( __value );                }
            ini_keyvalue_t& operator=( unsigned short     __value ) { return set_ivalue<unsigned short    >( __value );                }
            ini_keyvalue_t& operator=( int                __value ) { return set_ivalue<int               >( __value );                }
            ini_keyvalue_t& operator=( unsigned int       __value ) { return set_ivalue<unsigned int      >( __value );                }
            ini_keyvalue_t& operator=( long               __value ) { return set_ivalue<long              >( __value );                }
            ini_keyvalue_t& operator=( unsigned long      __value ) { return set_ivalue<unsigned long     >( __value );                }
            ini_keyvalue_t& operator=( long long          __value ) { return set_ivalue<long long         >( __value );                }
            ini_keyvalue_t& operator=( unsigned long long __value ) { return set_ivalue<unsigned long long>( __value );                }
            ini_keyvalue_t& operator=( float              __value ) { return set_fvalue( __value ,  6 );                               }
            ini_keyvalue_t& operator=( double             __value ) { return set_fvalue( __value , 16 );                               }

#pragma endregion basic_data_type_write_operator

        // public interfaces
        public:
            inline const std::string& key( void ) const;
            inline const std::string& value( void ) const;
            inline bool empty( void ) const { return m_str_value.empty(); }
            void set_value( const std::string& );

        protected:
            void sl_nbabae_set_value( const std::string& );

        protected:
            std::string m_str_kname; // key name
            std::string m_str_value; // key value

    }; // namespace ini_keyvalue_t

    /**
     * @class ini_section_t
     * @brief section node (in ini file) class
     */
    class ini_section_t: public ini_node_t {
        friend class ini_file;

        // common data types
        protected:
            typedef std::list <ini_node_t*>                            lst_node_t;
            typedef std::map  <std::string , ini_keyvalue_t* , cswc_t> map_ndkv_t;

        // common data types
        public:
            typedef lst_node_t::iterator       iterator;
            typedef lst_node_t::const_iterator const_iterator;

        // common invoking
        protected:
            static bool is_ntype( const std::string& );
            static ini_node_t* try_create( const std::string& , ini_node_t* );

        // construcor / destructor
        protected:
            ini_section_t( ini_node_t* owner_ptr )
                : ini_node_t( SECTION , owner_ptr ) {}
            
            virtual ~ini_section_t( void );

        // overrides
        public:
            virtual const ini_node_t& operator>>( std::ostream& ) const;

        // overrides: operator
        public:
            ini_keyvalue_t& operator[]( const std::string& );

        // public interfaces
        public:
            inline const std::string& name( void ) const;
            inline size_t size( void ) const;
            inline bool empty() const;
            inline bool has_end_nilline( void );

        // iterator
        public:
            inline iterator       begin( void )       { return m_lst_node.begin(); }
            inline const_iterator begin( void ) const { return m_lst_node.begin(); }

            inline iterator       end( void )       { return m_lst_node.end();   }
            inline const_iterator end( void ) const { return m_lst_node.end();   }

            inline iterator       begin_kv( void );
            inline const_iterator begin_kv( void ) const;

            inline iterator       next_kv( iterator );
            inline const_iterator next_kv( const_iterator ) const;

        // inner invoking
        protected:
            bool push_node( ini_node_t* );
            ini_keyvalue_t* find_knode( const std::string& ) const;
            size_t pop_tail_comment( std::list <ini_node_t*>& , bool );

        // data members
        protected:
            std::string m_str_name; // section name
            lst_node_t  m_lst_node; // section node list
            map_ndkv_t  m_map_ndkv; // section key value node index

    }; // class ini_section_t

    class ini_file: public ini_node_t {

        // common data types
        protected:
            typedef std::list <ini_section_t*>                        lst_section_t;
            typedef std::map  <std::string , ini_section_t* , cswc_t> map_section_t;

        // common invoking
        protected:
            static ini_node_t* make_node( const std::string& , ini_file* );

        // constructor / destructor
        public:
            ini_file( void )
                : ini_node_t( FILEROOT , NULL )
                , m_bt_dirty( false ) {}

            ini_file( const std::string& __str_filepath )
                : ini_node_t( FILEROOT , NULL )
                , m_bt_dirty( false ) { load( __str_filepath ); }

            ~ini_file( void ) { release(); }

        // overrides
        public:
            virtual const ini_node_t& operator>>( std::ostream& ) const;

            virtual bool is_dirty( void ) const;
            virtual void set_dirty( bool );

        // overrides: operator
        public:
            ini_file&      operator<<( std::istream& );
            ini_section_t& operator[]( const std::string& );

        // public interfaces
        public:
            bool load( const std::string& );
            void release( void );

            inline const std::string& filepath( void ) const;
            inline size_t sect_count( void ) const;

            bool dump( const std::string& );

        // inner invoking
        protected:
            ini_section_t* find_sect( const std::string& ) const;
            ini_section_t* push_sect( ini_section_t* , ini_section_t* );

        // data members
        protected:
            bool          m_bt_dirty; // dirty signature
            std::string   m_str_path; // file path
            std::string   m_str_head; // char-set infos at file-begin
            lst_section_t m_lst_sect; // section node list under file-root
            map_section_t m_map_sect; // each sections' node index

    }; // class ini_file

    inline std::istream& operator>>( std::istream& , ini_file& );

}; // namespace rena

#endif