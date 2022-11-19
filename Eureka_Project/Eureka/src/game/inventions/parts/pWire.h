#pragma once

#include "engine/concepts/Grid3D.h"
#include "game/inventions/Part.h"
#include "PowerSupplier.h"
#include "PowerReceiver.h"

class WireConnection;

class pWire : public Part
{
private:
	std::vector<Grid3Pos> m_SourcePositions;
	WireConnection* m_WireConnection;
	int m_FaceTex;
	int m_PoweredFaceTex;
public:
	pWire();
	~pWire();

	std::string GetPath() override;
	float* GetUVs() override;
	WireConnection* GetWireConnection() const { return m_WireConnection; }
	void ConnectToWires(Invention* invention);
	void SetWireConnection(WireConnection* wireConnection);
	void FindConnections(std::vector<Grid3Pos>& path, Invention* invention);

	void SetPoweredState(bool state);

	void Action(Invention* invention) override;
	void OnRemove(Invention* invention) override;
	void OnEnvironmentChange(Invention* invention) override;
};