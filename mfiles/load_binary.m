function A = load_binary( file_path );

fp = fopen( file_path, 'rb' );

btag = fread( fp, 1, 'int' );

w  = fread( fp, 1, 'int' )
h  = fread( fp, 1, 'int' )
ch = fread( fp, 1, 'int' )
tp = fread( fp, 1, 'int' )

assert( tp == 2, 'type should be float' );
assert( ch == 1, '#channels should be 1' );

A = fread( fp, w*h, 'float' );
A = reshape( A, w, h )';


fclose( fp );


