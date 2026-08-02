#include "SeamCarve.h"
#include <climits>
#include <cstdlib>

// --- Disruption computation ---

std::vector<std::vector<int>> calculateDisruption(const RGBImage &img)
{
    int h = img.height;
    int w = img.width;
    std::vector<std::vector<int>> d(h, std::vector<int>(w, 0));

    for (int y = 0; y < h; ++y)
    {
        for (int x = 0; x < w; ++x)
        {
            uint8_t cr, cg, cb;
            getPixel(img, x, y, cr, cg, cb);

            int disruption = 0;
            // examine up to 8 neighbors
            for (int dy = -1; dy <= 1; ++dy)
            {
                for (int dx = -1; dx <= 1; ++dx)
                {
                    if (dx == 0 && dy == 0)
                        continue;
                    int nx = x + dx, ny = y + dy;
                    if (nx >= 0 && nx < w && ny >= 0 && ny < h)
                    {
                        uint8_t nr, ng, nb;
                        getPixel(img, nx, ny, nr, ng, nb);
                        disruption += std::abs(cr - nr) +
                                      std::abs(cg - ng) +
                                      std::abs(cb - nb);
                    }
                }
            }
            d[y][x] = disruption;
        }
    }
    return d;
}

// --- Vertical seam ---

std::vector<int> findVerticalSeam(const std::vector<std::vector<int>> &d,
                                  int rows, int cols)
{
    // 1-based indexing for DP convenience
    std::vector<std::vector<int>> D(rows + 1, std::vector<int>(cols + 1, 0));
    std::vector<std::vector<int>> prev(rows + 1, std::vector<int>(cols + 1, 0));

    for (int j = 1; j <= cols; ++j)
    {
        D[1][j] = d[0][j - 1];
        prev[1][j] = 0;
    }

    for (int i = 2; i <= rows; ++i)
    {
        for (int j = 1; j <= cols; ++j)
        {
            int min_val = D[i - 1][j] + d[i - 1][j - 1];
            int prev_state = j;

            if (j > 1)
            {
                int cand = D[i - 1][j - 1] + d[i - 1][j - 1];
                if (cand < min_val)
                {
                    min_val = cand;
                    prev_state = j - 1;
                }
            }
            if (j < cols)
            {
                int cand = D[i - 1][j + 1] + d[i - 1][j - 1];
                if (cand < min_val)
                {
                    min_val = cand;
                    prev_state = j + 1;
                }
            }

            D[i][j] = min_val;
            prev[i][j] = prev_state;
        }
    }

    // backtrace
    int min_seam_cost = INT_MAX;
    int min_j = 1;
    for (int j = 1; j <= cols; ++j)
    {
        if (D[rows][j] < min_seam_cost)
        {
            min_seam_cost = D[rows][j];
            min_j = j;
        }
    }

    std::vector<int> seam;
    seam.push_back(min_j);
    for (int i = rows - 1; i >= 1; --i)
    {
        min_j = prev[i + 1][min_j];
        seam.insert(seam.begin(), min_j);
    }
    return seam;
}

// --- Horizontal seam ---

std::vector<int> findHorizontalSeam(const std::vector<std::vector<int>> &d,
                                    int rows, int cols)
{
    std::vector<std::vector<int>> D(rows + 1, std::vector<int>(cols + 1, 0));
    std::vector<std::vector<int>> prev(rows + 1, std::vector<int>(cols + 1, 0));

    for (int i = 1; i <= rows; ++i)
    {
        D[i][1] = d[i - 1][0];
        prev[i][1] = 0;
    }

    for (int j = 2; j <= cols; ++j)
    {
        for (int i = 1; i <= rows; ++i)
        {
            int min_val = D[i][j - 1] + d[i - 1][j - 1];
            int prev_state = i;

            if (i > 1)
            {
                int cand = D[i - 1][j - 1] + d[i - 1][j - 1];
                if (cand < min_val)
                {
                    min_val = cand;
                    prev_state = i - 1;
                }
            }
            if (i < rows)
            {
                int cand = D[i + 1][j - 1] + d[i - 1][j - 1];
                if (cand < min_val)
                {
                    min_val = cand;
                    prev_state = i + 1;
                }
            }

            D[i][j] = min_val;
            prev[i][j] = prev_state;
        }
    }

    // backtrace
    int min_seam_cost = INT_MAX;
    int min_i = 1;
    for (int i = 1; i <= rows; ++i)
    {
        if (D[i][cols] < min_seam_cost)
        {
            min_seam_cost = D[i][cols];
            min_i = i;
        }
    }

    std::vector<int> seam;
    seam.push_back(min_i);
    for (int j = cols - 1; j >= 1; --j)
    {
        min_i = prev[min_i][j + 1];
        seam.insert(seam.begin(), min_i);
    }
    return seam;
}

// --- Seam removal helpers ---

RGBImage removeVerticalSeam(const RGBImage &img,
                            const std::vector<int> &seam)
{
    int h = img.height, w = img.width;
    RGBImage out;
    out.width = w - 1;
    out.height = h;
    out.data.resize(3 * out.width * out.height);

    for (int y = 0; y < h; ++y)
    {
        int skipCol = seam[y] - 1;
        int newX = 0;
        for (int x = 0; x < w; ++x)
        {
            if (x == skipCol)
                continue;
            uint8_t r, g, b;
            getPixel(img, x, y, r, g, b);
            setPixel(out, newX, y, r, g, b);
            ++newX;
        }
    }
    return out;
}

RGBImage removeHorizontalSeam(const RGBImage &img,
                              const std::vector<int> &seam)
{
    int h = img.height, w = img.width;
    RGBImage out;
    out.width = w;
    out.height = h - 1;
    out.data.resize(3 * out.width * out.height);

    for (int x = 0; x < w; ++x)
    {
        int skipRow = seam[x] - 1;
        int newY = 0;
        for (int y = 0; y < h; ++y)
        {
            if (y == skipRow)
                continue;
            uint8_t r, g, b;
            getPixel(img, x, y, r, g, b);
            setPixel(out, x, newY, r, g, b);
            ++newY;
        }
    }
    return out;
}
