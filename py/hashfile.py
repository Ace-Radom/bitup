import hashlib


def hashfile_md5( filename , block_size = 1024 * 64 ):
    with open( filename , 'rb' ) as rFile:
        hash = hashlib.md5()
        while True:
            rData = rFile.read( block_size )
            if not rData:
                break
            hash.update( rData )
        rFile.close()
        return hash.hexdigest()

def hashfile_sha1( filename , block_size = 1024 * 64 ):
    with open( filename , 'rb' ) as rFile:
        hash = hashlib.sha1()
        while True:
            rData = rFile.read( block_size )
            if not rData:
                break
            hash.update( rData )
        rFile.close()
        return hash.hexdigest()

def hashfile_sha224( filename , block_size = 1024 * 64 ):
    with open( filename , 'rb' ) as rFile:
        hash = hashlib.sha224()
        while True:
            rData = rFile.read( block_size )
            if not rData:
                break
            hash.update( rData )
        rFile.close()
        return hash.hexdigest()

def hashfile_sha256( filename , block_size = 1024 * 64 ):
    with open( filename , 'rb' ) as rFile:
        hash = hashlib.sha256()
        while True:
            rData = rFile.read( block_size )
            if not rData:
                break
            hash.update( rData )
        rFile.close()
        return hash.hexdigest()

def hashfile_sha384( filename , block_size = 1024 * 64 ):
    with open( filename , 'rb' ) as rFile:
        hash = hashlib.sha384()
        while True:
            rData = rFile.read( block_size )
            if not rData:
                break
            hash.update( rData )
        rFile.close()
        return hash.hexdigest()

def hashfile_sha512( filename , block_size = 1024 * 64 ):
    with open( filename , 'rb' ) as rFile:
        hash = hashlib.sha512()
        while True:
            rData = rFile.read( block_size )
            if not rData:
                break
            hash.update( rData )
        rFile.close()
        return hash.hexdigest()