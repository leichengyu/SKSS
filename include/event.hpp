#ifndef EVENT_H_
#define EVENT_H_


namespace skss {

	class EventLoop;

	class Event
	{
		public:
			Event();
			~Event();
			void setEventLoop(EventLoop* eventloop) {eventloop = eventloop;}
		protected:
			EventLoop* eventloop;
	};

	class FileEvent:public Event {
		public:
			FileEvent();
			~FileEvent();

			inline int getMask() const {return this->mask;}
			inline void setMask(int mask) {this->mask = mask;}

		private:
			int mask;
	};

	class FiredEvent: public Event {
		public:
			FiredEvent();
			~FiredEvent();
			inline int getMask() const {return this->mask;}
			inline void setMask(int mask) {this->mask = mask;}
			inline int getFd() const {return this->fd;}
			inline void setFd(int fd) {this->fd = fd;}
		private:
			int fd;
			int mask;
	};
};
#endif