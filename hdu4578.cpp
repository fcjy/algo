#include <cstdio>
#include <cstring>
#include <algorithm>

const int maxn = 100100;
const int mod = 10007;

using namespace std;

inline void add( int& x, int y )
{
	x = (x+y)%mod;
}

inline int san( int x )
{
	return x*(x*x%mod)%mod;
}

struct ST
{
	int ll, rr;
	int v[3];
	int a, b, r;

	inline int mid()
	{
		return (ll+rr)/2;
	}
	inline int len()
	{
		return rr-ll+1;
	}
	void fa( int x )
	{
		if( x == 1 )	return;

		v[0] = v[0]*x%mod;
		v[1] = v[1]*(x*x%mod)%mod;
		v[2] = v[2]*san(x)%mod;
	
		a = a*x%mod;
		b = b*x%mod;
	}
	void fb( int x )
	{
		if( x == 0 )	return;

		int L = len();
		add(v[2], san(x)*L%mod);
		add(v[2], x*3*v[1]%mod);
		add(v[2], (x*x*3%mod)*v[0]%mod);
		add(v[1], (x*x%mod)*L%mod);
		add(v[1], 2*x*v[0]%mod);
		add(v[0], L*x%mod);
	
		b = (b+x)%mod;
	}
	void R( int x )
	{
		int L = len();
		v[0] = L*x%mod;
		v[1] = L*(x*x%mod)%mod;
		v[2] = L*san(x)%mod;
		
		a = 1, b = 0;
		r = x;
	}
} p[maxn*4];

void build( int s, int e, int n )
{
	p[n].ll = s, p[n].rr = e;
	p[n].a = p[n].b = 0;
	p[n].r = -1;
	memset(p[n].v, 0, sizeof(int)*3);

	if( s != e )
	{
		int m = p[n].mid();
		build(s, m, n*2+1);
		build(m+1, e, n*2+2);
	}
}

void D( int n )
{
	if( p[n].r != -1 )
	{
		p[n*2+1].R(p[n].r);
		p[n*2+2].R(p[n].r);
		p[n].r = -1;
	}

	p[n*2+1].fa(p[n].a);
	p[n*2+1].fb(p[n].b);
	p[n*2+2].fa(p[n].a);
	p[n*2+2].fb(p[n].b);
	p[n].a = 1, p[n].b = 0;
}

inline void U( int n )
{
	for( int i = 0; i < 3; ++i )
		p[n].v[i] = (p[n*2+1].v[i]+p[n*2+2].v[i])%mod;
}

void up( int s, int e, int va, int vb, int n )
{
	if( p[n].ll == s && p[n].rr == e )
	{
		p[n].fa(va);
		p[n].fb(vb);
		return;
	}

	D(n);

	int m = p[n].mid();
	if( m >= e )
		up(s, e, va, vb, n*2+1);
	else if( m < s )
		up(s, e, va, vb, n*2+2);
	else
	{
		up(s, m, va, vb, n*2+1);
		up(m+1, e, va, vb, n*2+2);
	}

	U(n);
}

void re( int s, int e, int v, int n )
{
	if( p[n].ll == s && p[n].rr == e )
	{
		p[n].R(v);
		return;
	}

	D(n);

	int m = p[n].mid();
	if( m >= e )
		re(s, e, v, n*2+1);
	else if( m < s )
		re(s, e, v, n*2+2);
	else
	{
		re(s, m, v, n*2+1);
		re(m+1, e, v, n*2+2);
	}

	U(n);
}

int Q( int s, int e, int k, int n )
{
	if( p[n].ll == s && p[n].rr == e )
		return p[n].v[k];
	
	D(n);

	int m = p[n].mid();
	if( m >= e )
		return Q(s, e, k, n*2+1);
	else if( m < s )
		return Q(s, e, k, n*2+2);
	else
		return (Q(s, m, k, n*2+1) + Q(m+1, e, k, n*2+2))%mod;
}

int main()
{
	int N, M, k, s, e, v;

	while( scanf("%d %d", &N, &M), N )
	{
		build(0, N-1, 0);

		while( M-- )
		{
			scanf("%d %d %d %d", &k, &s, &e, &v);
			s--, e--;

			if( k == 1 )
				up(s, e, 1, v, 0);		
			else if( k == 2 )
				up(s, e, v, 0, 0);
			else if( k == 3 )
				re(s, e, v, 0);
			else if( k == 4 )
				printf("%d\n", Q(s, e, v-1, 0));
		}
	}

	return 0;
}
