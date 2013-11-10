#ifndef DX11SANDBOX_LIGHT_H
#define DX11SANDBOX_LIGHT_H
#include "CommonTypes.h"
#include "Cullable.h"

namespace Dx11Sandbox
{

	class Light: public Cullable
	{
	public:
		Light(void);
		virtual ~Light(void);

		virtual CullableType GetCullableType() const;
	};

};
#endif
