#pragma once

#include"tt_Factory.h"
#include "tt_Image.h"
#include "tt_Parameterized_Quadrilateral_Affine.h"
#include "tt_Heatmap_Roamer.h"
#include <utility>
namespace foundations
{
	//class Parameterized_Quadrilateral_Affine;
	class Parameterized_Transform;
	//class Heatmap_Roamer;
	//template<class ...T>
	//class Gene;
	template<class ...T>
	struct List;

	template<class H, class ...T>
	struct List<H, T...>
	{
		using Head = H;
		using Tail = List<T...>;
	};

	template<class ...T>
	class FOUNDATIONS_EXPORTS Gene:
		virtual public T...,
		public Factory<Gene<T...>>
	{
	public:
		typedef Gene<T...> Self;
		FACTORY_DECLARE
		PIMPL_DECLARE
	protected:
		Gene();
	public:
		virtual ~Gene();
		/*typedef Gene Self;
		template<class U = Gene<T...>>
		typename std::enable_if<!std::is_base_of<Heatmap_Roamer, U>::value, void>::type set_heatmap(Image& value);
		template<class U = Gene<T...>>
		typename std::enable_if<std::is_base_of<Heatmap_Roamer, U>::value, void>::type set_heatmap(Image& value);*/
		virtual std::string get_class_name() const
		{
			return "Gene";
		}
		POINTER_TYPES
	public:

		//////////////////////////////////////////////////////////////////////
		template<class Name>
		static constexpr bool Has = std::is_base_of < Name, Gene<T...> >::value;
		//////////////////////////////////////////////////////////////////////


		//////////////////////////////////////////////////////////////////////
		template<class Name >
		void work(Name& output) const
		{
			return;
		}
		template<>
		void work<Parameterized_Quadrilateral_Affine>(Parameterized_Quadrilateral_Affine& output) const
		{
			output.set_angle_range(this->get_angle_range());
		}
		template<>
		void work<Heatmap_Roamer>(Heatmap_Roamer& output) const
		{
			output.set_heatmap(*this->get_heatmap());
			output.set_neighborhood_range(this->get_neighborhood_range());
			output.set_threshold_range(this->get_threshold_range());
		}
		//////////////////////////////////////////////////////////////////////


		//////////////////////////////////////////////////////////////////////
		template<class Base>
		typename std::enable_if<Has<Base>, void>::type try_define_transform(const Parameters_Type& parameters)
		{
			Base::define_transform(parameters);
		}
		template<class Base>
		typename	std::enable_if<!Has<Base>, void>::type try_define_transform(const Parameters_Type&) {}
		//////////////////////////////////////////////////////////////////////



		//////////////////////////////////////////////////////////////////////
		template<class Base>
		typename std::enable_if<Has<Base>, void>::type do_job(Base& obj) const
		{
			work<Base>(obj);
		}

		template<class Base>
		typename std::enable_if<!Has<Base>, void>::type do_job(...) const
		{
			return;
		}
		//////////////////////////////////////////////////////////////////////



		//////////////////////////////////////////////////////////////////////
		template<class L>
		void foo_impl(Range_Type& output) const 
		{
			auto base_range = L::Head::get_range_internal();
			//auto base_range = Base::get_range_internal();
			for (auto el : base_range)
				output.push_back(el);

			foo_impl<typename L::Tail>(output);
		}

		template<>
		void foo_impl<List<>>(Range_Type&) const {}
		//////////////////////////////////////////////////////////////////////



		//////////////////////////////////////////////////////////////////////
		template<class T>
		typename std::enable_if<std::is_base_of<Transform, T>::value, void>::type kek(const Parameters_Type & parameters) 
		{
			T::define_transform(parameters);
		}

		template<class T>
		typename std::enable_if<!std::is_base_of<Transform, T>::value, void>::type kek(const Parameters_Type & parameters) 
		{
			T::set_parameters_internal(parameters);
		}

		template<class L>
		void lol(const Parameters_Type& parameters,size_t i = 0) 
		{
			Parameters_Type transform_parameters;
			for (size_t j = i; i < L::Head::get_range_internal().size(); j++)
				transform_parameters.push_back(parameters[i]);
			
			i += L::Head::get_range_internal().size();
			kek<typename L::Head>(transform_parameters);
			lol<typename L::Tail>(parameters, i);
		}

		template<>
		void lol<List<>>(const Parameters_Type&, size_t)  {}
		//////////////////////////////////////////////////////////////////////



		//////////////////////////////////////////////////////////////////////
		template<class Base>
		typename std::enable_if<Has<Base>, void>::type try_get_range(Range_Type& output) const
		{
			auto base_range = Base::get_range_internal();
			for (auto el : base_range)
				output.push_back(el);
		}
		template<class Base>
		typename std::enable_if<!Has<Base>, void>::type try_get_range(Range_Type&) const {}
		//////////////////////////////////////////////////////////////////////




		//////////////////////////////////////////////////////////////////////
		template <class baseT, typename funcT, class ...Args>
		typename std::enable_if<std::is_base_of<baseT, Gene>::value, void>::type call_if_base(funcT func, Args&&... args) {
			(dynamic_cast<baseT&>(*this).*func)(std::forward<Args>(args)...);
		}
		template <class baseT, typename funcT, class ...Args>
		typename std::enable_if<!std::is_base_of<baseT, Gene>::value, void>::type call_if_base(funcT, Args&&...) {
		}
		//////////////////////////////////////////////////////////////////////

		virtual 	std::shared_ptr<Parameterized_Transform> generate() const ;
		virtual void define_transform(const Parameters_Type &);
		virtual Range_Type get_range_internal() const;
		virtual void set_parameters_internal(const Parameters_Type&);
		virtual void reset_parameters();

	};

}