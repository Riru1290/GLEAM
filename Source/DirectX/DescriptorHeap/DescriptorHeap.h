#pragma once

/// @brief ディスクリプタヒープ既定クラス
class DescriptorHeap
{
public:

	DescriptorHeap() = default;
	virtual ~DescriptorHeap() = default;

	/// @brief  ヒープ作成
	/// @param useCount 使用個数
	/// @return 作成に成功したか
	[[nodiscard]] virtual bool Create(int useCount) = 0;

	/// @brief CPU側アドレスを返す関数
	/// @param number 登録番号
	/// @return CPU側アドレス
	[[nodiscard]] D3D12_CPU_DESCRIPTOR_HANDLE GetCPUAddress(int number);

	/// @brief GPU側アドレスを返す関数
	/// @param number 登録番号
	/// @return GPU側アドレス
	[[nodiscard]] D3D12_GPU_DESCRIPTOR_HANDLE GetGPUAddress(int number);

	/// @brief ディスクリプタヒープ実体取得
	/// @return ディスクリプタヒープ
	[[nodiscard]] ComPtr<ID3D12DescriptorHeap> GetDescriptorHeap()const { return heap_; }

	/// @brief 指定レジスタ番号のディスクリプタを解放
	/// @param registNo レジスタ番号
	void ReleaseDescriptor(int registNo);


	// テスト
	int GetRegisterNum()
	{
		int use = 0;
		for (auto& [no, flag] : registNumber_)
		{
			if (flag) use++;
		}
		return use;
	}

protected:

	ComPtr<ID3D12DescriptorHeap> heap_ = nullptr;	/// @brief ヒープ

	int useCount_ = 0;								/// @brief 使用個数

	int incrementSize_ = 0;							/// @brief インクリメントサイズ

	map<int, bool> registNumber_;			/// @brief レジスタ番号管理
};

