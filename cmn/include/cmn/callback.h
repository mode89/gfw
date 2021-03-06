#ifndef __CMN_CALLBACK_H__
#define __CMN_CALLBACK_H__

namespace Cmn
{
	// Callback interface
	class ICallback
	{
	public:
		virtual void operator()() = 0;
	};

	// Callback template
	template<class Receiver>
	class CallbackTemplate: public ICallback
	{
	public:
		typedef void (Receiver::*Action)();
		CallbackTemplate(Receiver * receiver, Action action):
			m_receiver(receiver), m_action(action) { }

		// Execute callback
		virtual void operator()()
		{
			(m_receiver->*m_action)();
		}

	private:
		Receiver * m_receiver;
		Action m_action;
	};

	// Construct a callback
	#define Callback(receiver, type, action) \
		CallbackTemplate<type>(receiver, &type::action)

} // namespace Cmn

#endif // __CMN_CALLBACK_H__
