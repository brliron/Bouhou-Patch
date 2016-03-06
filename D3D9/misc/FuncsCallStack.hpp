#ifndef FUNCS_CALL_STACK_HPP_
# define FUNCS_CALL_STACK_HPP_

# include	<tuple>
# include	<queue>

// Template-independant intrerface for the FuncCall class
class	IFuncCall
{
public:
  virtual ~IFuncCall(){}
  virtual void	call() const = 0;
};



/*
** Stores a function call and its arguments, and allow you to call
** this function later without re-specifying the arguments.
*/
template<typename FuncType, typename... Args>
class	FuncCall : public IFuncCall
{
private:
  FuncType		func;
  std::tuple<Args...>	args;

  // Retrieve the stored arguments one by one
  template<typename... TempArgs>
  void	_call(TempArgs... args) const
  {
    this->_call(args..., std::get<sizeof...(TempArgs)>(this->args));
  }

  // Do the actual function call
  template<Args...>
  void	_call(Args... args) const
  {
    this->func(args...);
  }

public:
  FuncCall(FuncType func, Args... args)
    : func(func), args(args...)
  {}

  // Wrapper around _call<> (because a template function can't be virtual).
  void	call() const
  {
    this->_call();
  }
};



/*
** A stack of FuncCall pointers. Has the same interface than std::stack, except front(),
** which was replaced with call(), and push, which was removed (emplace is enough).
*/
class	FuncsCallStack
{
private:
  std::queue<IFuncCall*>	funcs;

public:
  template<typename FuncType, typename... Args>
  void		emplace(FuncType func, Args... args)
  {
    this->funcs.push(new FuncCall<FuncType, Args...>(func, args...));
  }

  // Calls the function at the top of the stack.
  bool		call() const
  {
    if (this->funcs.empty())
      return false;
    this->funcs.front()->call();
    return true;
  }

  void		pop()
  {
    if (!this->funcs.empty())
      {
	delete this->funcs.front();
	this->funcs.pop();
      }
  }

  bool		empty() const
  {
    return this->funcs.empty();
  }

  unsigned long	size() const
  {
    return this->funcs.empty();
  }
};

#endif /* !FUNCS_CALL_STACK_HPP_ */
