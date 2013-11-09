#ifndef DX11SANDBOX_CULLABLE_H
#define DX11SANDBOX_CULLABLE_H

namespace Dx11Sandbox
{

	enum CullableType
	{
		CULLABLE_GEOMETRY,
		CULLABLE_LIGHT

	};

	class Cullable
	{
	public:

		Cullable(void);
		virtual ~Cullable(void);

		virtual CullableType GetCullableType() const = 0;

	};

};
#endif