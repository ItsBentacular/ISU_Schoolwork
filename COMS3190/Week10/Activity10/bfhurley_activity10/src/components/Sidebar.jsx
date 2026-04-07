import logo from "../assets/logo.png";

const SideNavbar = ({categories, active, setActive, query, setQuery}) => {
    const handleChange = (e) => {
        setQuery(e.target.value);
    }
    return (
        <div className=" w-100 bg-[#0f172a] pt-80 text-white flex flex-col items-center min-h-screen p-6">
            <h1 className="text-4xl font-bold tracking-wide mb-4 text-center">
                Product Catalog
            </h1>
            <img src={logo} 
            alt="logo"
            className="w-40 mb-6 object-contain" />
            <div className="py-10"> 
                <input
                    type="search" 
                    value={query} 
                    onChange={handleChange}
                    placeholder="Search products..." 
                    className="w-[250px] p-2.5 rounded-lg bg-gray-700 text-white"
                />
            </div>
            <p className="text-xs uppercase tracking-widest text-gray-400 mb-4 text-center">
                Categories
            </p>
            <div className="flex flex-wrap justify-center gap-3">
                {categories.map((cat) => (
                    <button 
                    onClick={() => setActive(cat)}
                    key={cat} 
                    className={`px-4 py-2 rounded-full text-sm font-medium transition
                        ${
                            active === cat
                                ? "bg-amber-500 text-black shadow"
                                : "bg-slate-700 text-gray-200 hover:bg-slate-600"
                        }`}
                    >
                        {cat}
                    </button>
                ))}
            </div>
        </div>
    );
};

export default SideNavbar;
