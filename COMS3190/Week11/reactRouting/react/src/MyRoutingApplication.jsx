import { BrowserRouter as Router, Route, Routes, useNavigate } from 'react-router-dom';

const MyApp = () => {
    return (
        <Router>
            <Routes>
                <Route path="/view1" element={<View1 />} />
                <Route path="/view2" element={<View2 />} />
                <Route path="/view3" element={<View3 />} />
                <Route path="/" element={<View1 />} /> {/* Default view */}
            </Routes>
        </Router>
    );
};
const View1 = () => {
    let navitate = useNavigate();
    return (
        <div>
            <h1>View 1 <button onClick={() => Navigate('/view2')}>Go to View 2</button></h1>
            <img src="https://images.pexels.com/photos/36029/aroni-arsa-children-little.jpg" width={200} />
        </div>
    );
};
const View2 = () => {
    let navitate = useNavigate();
    return (
        <div>
            <h1>View 2 <button onClick={() => Navigate('/view3')}>Go to View 3</button></h1>
            <img src="https://images.pexels.com/photos/1043474/pexels-photo-1043474.jpeg" width={200} />
        </div>
    );
};
const View3 = () => {
    let navitate = useNavigate();
    return (
        <div>
            <h1>View 3 <button onClick={() => Navigate('/view1')}>Go to View 1</button></h1>
            <img src="https://images.pexels.com/photos/1631181/pexels-photo-1631181.jpeg" width={200} />
        </div>
    );
};