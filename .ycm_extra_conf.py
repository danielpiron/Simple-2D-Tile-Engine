def FlagsForFile(filename, **kwargs):
    return {
        'flags': ['-x', 'c++', '-Wall', '-std=c++11',
                  '-I/usr/local/include', '-L/usr/loca/lib'],
    }
