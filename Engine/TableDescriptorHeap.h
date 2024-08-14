#pragma once
class TableDescriptorHeap
{

public:
	void Init(uint32 count);

	void Clear();

	// Desc Heap -> Table Descriptor Heap
	void SetCBV(D3D12_CPU_DESCRIPTOR_HANDLE srcHandle, CBV_REGISTER reg);
	void SetSRV(D3D12_CPU_DESCRIPTOR_HANDLE srcHandle, SRV_REGISTER reg);

	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle(CBV_REGISTER reg);
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle(SRV_REGISTER reg);

	ComPtr<ID3D12DescriptorHeap> GetDescriptorHeap() { return _descHeap; }

	// SetGraphicsRootDescriptorTable : GPU가 해당 Descriptor Table 을 쓸수 있도록 세팅
	// Mesh 를 여러개 그리기 위한 구조
	void CommitTable();

private:
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle(uint8 reg);


private :

	ComPtr<ID3D12DescriptorHeap> _descHeap;
	uint64					_handleSize = 0;
	uint64					_groupSize = 0;
	uint64					_groupCount = 0;

	uint32					_currentGroupIndex = 0;
};

