// �����Ϳ������á�����ࡱ�����ļ�������ɵ� IDispatch ��װ��

#import "chart.ocx" no_namespace
// CEnvironment ��װ��

class CEnvironment : public COleDispatchDriver
{
public:
	CEnvironment(){} // ���� COleDispatchDriver Ĭ�Ϲ��캯��
	CEnvironment(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CEnvironment(const CEnvironment& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// ����
public:

	// ����
public:


	// IEnvironment ����
public:
	BOOL get_NoPromptDBConnect()
	{
		BOOL result;
		InvokeHelper(0x2f, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_NoPromptDBConnect(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x2f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_IEPrintWithPage()
	{
		BOOL result;
		InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_IEPrintWithPage(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x2, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	void InternalRepaint()
	{
		InvokeHelper(0x1, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	BOOL get_MouseWheelScroll()
	{
		BOOL result;
		InvokeHelper(0x3, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_MouseWheelScroll(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x3, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	CString get_DecimalSeparator()
	{
		CString result;
		InvokeHelper(0x4, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}

	// IEnvironment ����
public:

};
