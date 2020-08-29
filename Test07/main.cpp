#include <iostream>
#include <queue>
#include <utility>
using namespace std;
static float CalculateH(float side)
{
    return ConvertToFloat(System.Math.Sin(DegreesToRadians(30)) * side);
}

static float CalculateR(float side)
{
    return ConvertToFloat(System.Math.Cos(DegreesToRadians(30)) * side);
}
static double DegreesToRadians(double degrees)
{
    return degrees * System.Math.PI / 180;
}
class Hex
{
    private System.Drawing.PointF[] points;
    private float side;
    private float h;
    private float r;
    private Hexagonal.HexOrientation orientation;
    private float x;
    private float y;
    ...

    private void CalculateVertices()
    {
        h = Hexagonal.Math.CalculateH(side);
        r = Hexagonal.Math.CalculateR(side);
        switch (orientation)
        {
        case Hexagonal.HexOrientation.Flat:
            // x,y coordinates are top left point

            points = new System.Drawing.PointF[6];
            points[0] = new PointF(x, y);
            points[1] = new PointF(x + side, y);
            points[2] = new PointF(x + side + h, y + r);
            points[3] = new PointF(x + side, y + r + r);
            points[4] = new PointF(x, y + r + r);
            points[5] = new PointF(x - h, y + r );
            break;
        case Hexagonal.HexOrientation.Pointy:
            //x,y coordinates are top center point

            points = new System.Drawing.PointF[6];
            points[0] = new PointF(x, y);
            points[1] = new PointF(x + r, y + h);
            points[2] = new PointF(x + r, y + side + h);
            points[3] = new PointF(x, y + side + h + h);
            points[4] = new PointF(x - r, y + side + h);
            points[5] = new PointF(x - r, y + h);
            break;
        default:
            throw new Exception("No HexOrientation defined for Hex object.");
        }
    }
}

public enum HexOrientation
{
    Flat = 0,
    Pointy = 1,
}
public class Board
{
    private Hexagonal.Hex[,] hexes;
    private int width;
    private int height;
    private int xOffset;
    private int yOffset;
    private int side;
    private float pixelWidth;
    private float pixelHeight;
    private Hexagonal.HexOrientation orientation;

    ...

    private void Initialize(int width, int height, int side,
                            Hexagonal.HexOrientation orientation,
                            int xOffset, int yOffset)
    {
        this.width = width;
        this.height = height;
        this.xOffset = xOffset;
        this.yOffset = yOffset;
        this.side = side;
        this.orientation = orientation;
        hexes = new Hex[height, width];
        //opposite of what we'd expect

        this.boardState = new BoardState();
        // short side
        float h = Hexagonal.Math.CalculateH(side);
        // long side
        float r = Hexagonal.Math.CalculateR(side);

        //
        // Calculate pixel info..remove?
        // because of staggering, need to add an extra r/h

        float hexWidth = 0;
        float hexHeight = 0;
        switch (orientation)
        {
        case HexOrientation.Flat:
            hexWidth = side + h;
            hexHeight = r + r;
            this.pixelWidth = (width * hexWidth) + h;
            this.pixelHeight = (height * hexHeight) + r;
            break;
        case HexOrientation.Pointy:
            hexWidth = r + r;
            hexHeight = side + h;
            this.pixelWidth = (width * hexWidth) + r;
            this.pixelHeight = (height * hexHeight) + h;
            break;
        default:
            break;
        }

        bool inTopRow = false;
        bool inBottomRow = false;
        bool inLeftColumn = false;
        bool inRightColumn = false;
        bool isTopLeft = false;
        bool isTopRight = false;
        bool isBotomLeft = false;
        bool isBottomRight = false;

        // i = y coordinate (rows), j = x coordinate
        //      (columns) of the hex tiles 2D plane

        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                // Set position booleans


                if (i == 0)
                {
                    inTopRow = true;
                }
                else
                {
                    inTopRow = false;
                }
                if (i == height - 1)
                {
                    inBottomRow = true;
                }
                else
                {
                    inBottomRow = false;
                }
                if (j == 0)
                {
                    inLeftColumn = true;
                }
                else
                {
                    inLeftColumn = false;
                }
                if (j == width - 1)
                {
                    inRightColumn = true;
                }
                else
                {
                    inRightColumn = false;
                }
                if (inTopRow && inLeftColumn)
                {
                    isTopLeft = true;
                }
                else
                {
                    isTopLeft = false;
                }
                if (inTopRow && inRightColumn)
                {
                    isTopRight = true;
                }
                else
                {
                    isTopRight = false;
                }
                if (inBottomRow && inLeftColumn)
                {
                    isBotomLeft = true;
                }
                else
                {
                    isBotomLeft = false;
                }
                if (inBottomRow && inRightColumn)
                {
                    isBottomRight = true;
                }
                else
                {
                    isBottomRight = false;
                }


                //
                // Calculate Hex positions
                //

                if (isTopLeft)
                {
                    //First hex
                    switch (orientation)
                    {
                    case HexOrientation.Flat:
                        hexes[0, 0] = new Hex(0 + h + xOffset,
                                              0 + yOffset, side, orientation);
                        break;
                    case HexOrientation.Pointy:
                        hexes[0, 0] = new Hex(0 + r + xOffset,
                                              0 + yOffset, side, orientation);
                        break;
                    default:
                        break;
                    }
                }
                else
                {
                    switch (orientation)
                    {
                    case HexOrientation.Flat:
                        if (inLeftColumn)
                        {
                            // Calculate from hex above
                            hexes[i, j] = new Hex(hexes[i - 1, j].
                                                  Points[(int)Hexagonal.FlatVertice.BottomLeft],
                                                  side, orientation);
                        }
                        else
                        {
                            // Calculate from Hex to the left
                            // and need to stagger the columns
                            if (j % 2 == 0)
                            {
                                // Calculate from Hex to left's
                                // Upper Right Vertice plus h and R offset
                                float x = hexes[i, j - 1].Points[
                                              (int)Hexagonal.FlatVertice.UpperRight].X;
                                float y = hexes[i, j - 1].Points[
                                              (int)Hexagonal.FlatVertice.UpperRight].Y;
                                x += h;
                                y -= r;
                                hexes[i, j] = new Hex(x, y, side, orientation);
                            }
                            else
                            {
                                // Calculate from Hex to left's Middle Right Vertice
                                hexes[i, j] = new Hex(hexes[i, j - 1].Points[
                                                          (int)Hexagonal.FlatVertice.MiddleRight],
                                                      side, orientation);
                            }
                        }
                        break;
                    case HexOrientation.Pointy:
                        if (inLeftColumn)
                        {
                            // Calculate from hex above and need to stagger the rows

                            if (i % 2 == 0)
                            {
                                hexes[i, j] = new Hex(hexes[i - 1, j].Points[
                                                          (int)Hexagonal.PointyVertice.BottomLeft],
                                                      side, orientation);
                            }
                            else
                            {
                                hexes[i, j] = new Hex(hexes[i - 1, j].Points[
                                                          (int)Hexagonal.PointyVertice.BottomRight],
                                                      side, orientation);
                            }
                        }
                        else
                        {
                            // Calculate from Hex to the left
                            float x = hexes[i, j - 1].Points[
                                          (int)Hexagonal.PointyVertice.UpperRight].X;
                            float y = hexes[i, j - 1].Points[
                                          (int)Hexagonal.PointyVertice.UpperRight].Y;
                            x += r;
                            y -= h;
                            hexes[i, j] = new Hex(x, y, side, orientation);
                        }
                        break;
                    default:
                        break;
                    }
                }
            }
        }
    }
}

public enum FlatVertice
{
    UpperLeft = 0,
    UpperRight = 1,
    MiddleRight = 2,
    BottomRight = 3,
    BottomLeft = 4,
    MiddleLeft = 5,
}

public enum PointyVertice
{
    Top = 0,
    UpperRight = 1,
    BottomRight = 2,
    Bottom = 3,
    BottomLeft = 4,
    TopLeft = 5,
}
public class GraphicsEngine
{
    private Hexagonal.Board board;
    private float boardPixelWidth;
    private float boardPixelHeight;
    private int boardXOffset;
    private int boardYOffset;
    ...

    public void Draw(Graphics graphics)
    {

        int width =  Convert.ToInt32(System.Math.Ceiling(board.PixelWidth));
        int height = Convert.ToInt32(System.Math.Ceiling(board.PixelHeight));
        // seems to be needed to avoid bottom and right from being chopped off

        width += 1;
        height += 1;

        //
        // Create drawing objects
        //

        Bitmap bitmap = new Bitmap(width, height);
        Graphics bitmapGraphics = Graphics.FromImage(bitmap);
        Pen p = new Pen(Color.Black);
        SolidBrush sb = new SolidBrush(Color.Black);

        //
        // Draw Board background
        //

        sb = new SolidBrush(board.BoardState.BackgroundColor);
        bitmapGraphics.FillRectangle(sb, 0, 0, width, height);

        //
        // Draw Hex Background
        //

        for (int i = 0; i < board.Hexes.GetLength(0); i++)
        {
            for (int j = 0; j < board.Hexes.GetLength(1); j++)
            {
                bitmapGraphics.FillPolygon(new SolidBrush(board.Hexes[i,j].
                                           HexState.BackgroundColor), board.Hexes[i, j].Points);
            }
        }

        //
        // Draw Hex Grid
        //

        p.Color = board.BoardState.GridColor;
        p.Width = board.BoardState.GridPenWidth;
        for (int i = 0; i < board.Hexes.GetLength(0); i++)
        {
            for (int j = 0; j < board.Hexes.GetLength(1); j++)
            {
                bitmapGraphics.DrawPolygon(p, board.Hexes[i, j].Points);
            }
        }

        //
        // Draw Active Hex, if present
        //

        if (board.BoardState.ActiveHex != null)
        {
            p.Color = board.BoardState.ActiveHexBorderColor;
            p.Width = board.BoardState.ActiveHexBorderWidth;
            bitmapGraphics.DrawPolygon(p, board.BoardState.ActiveHex.Points);
        }

        //
        // Draw internal bitmap to screen
        //
        graphics.DrawImage(bitmap, new Point(this.boardXOffset,
                                             this.boardYOffset));

        //
        // Release objects
        //
        bitmapGraphics.Dispose();
        bitmap.Dispose();
    }
    private void Form_Paint(object sender, PaintEventArgs e)
    {
        foreach (Control c in this.Controls)
        {
            c.Refresh();
        }
        if (graphicsEngine != null)
        {
            graphicsEngine.Draw(e.Graphics);
        }
        //Force the next Paint()

        this.Invalidate();
    }
    private void Form_MouseClick(object sender, MouseEventArgs e)
    {
        if (board != null && graphicsEngine != null)
        {
            //
            // need to account for any offset
            //

            Point mouseClick = new Point(e.X - graphicsEngine.BoardXOffset,
                                         e.Y - graphicsEngine.BoardYOffset);
            Hex clickedHex = board.FindHexMouseClick(mouseClick);
            if (clickedHex == null)
            {
                board.BoardState.ActiveHex = null;
            }
            else
            {
                board.BoardState.ActiveHex = clickedHex;
                if (e.Button == MouseButtons.Right)
                {
                    clickedHex.HexState.BackgroundColor = Color.Blue;
                }
            }
        }
    }
    public static bool InsidePolygon(PointF[] polygon, int N, PointF p)
    {
        int counter = 0;
        int i;
        double xinters;
        PointF p1,p2;
        p1 = polygon[0];
        for (i=1; i<=N; i++)
        {
            p2 = polygon[i % N];
            if (p.Y > System.Math.Min(p1.Y,p2.Y))
            {
                if (p.Y <= System.Math.Max(p1.Y,p2.Y))
                {
                    if (p.X <= System.Math.Max(p1.X,p2.X))
                    {
                        if (p1.Y != p2.Y)
                        {
                            xinters = (p.Y-p1.Y)*(p2.X-p1.X)/(p2.Y-p1.Y)+p1.X;
                            if (p1.X == p2.X || p.X <= xinters)
                                counter++;
                        }
                    }
                }
            }
            p1 = p2;
        }
        if (counter % 2 == 0)
            return false;
        else
            return true;
    }
}
