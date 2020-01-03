/^struct squashfs_/,/^}/{
	s/^struct \(squashfs_\([^[:space:]]*\)\).*/void sqfs_swapin_\2(struct \1 *s){/p;t decl
	s/};/}/p;t
	s/^[[:space:]]*__le\([[:digit:]]*\)[[:space:]]*\([a-z_]*\);$/sqfs_swapin\1_internal(\&s->\2);/p
}
d
:decl
s/{/;/
w swap-foo.h.inc
